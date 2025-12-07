#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
基金持仓修正工具
用于计算记账工具与实际持仓之间的差异，并生成修正交易
"""

import csv
import os
from decimal import Decimal, ROUND_HALF_UP
from typing import Dict, Tuple, Optional

class FundAdjustmentTool:
    def __init__(self, csv_file: str = "funds.csv"):
        """
        初始化基金数据
        从 CSV 文件读取基金信息

        CSV 格式：
        name,actual_amount,actual_cost,record_amount,record_cost
        示例基金，1000,1,1000,1
        """
        self.funds = {}
        self.csv_file = csv_file
        self.load_funds_from_csv()

    def load_funds_from_csv(self):
        """从 CSV 文件加载基金数据"""
        # 获取脚本所在目录
        script_dir = os.path.dirname(os.path.abspath(__file__))
        csv_path = os.path.join(script_dir, self.csv_file)

        if not os.path.exists(csv_path):
            print(f"警告：CSV 文件不存在：{csv_path}")
            print(f"请在脚本目录下创建 {self.csv_file} 文件")
            print("\n文件格式示例：")
            print("name,actual_amount,actual_cost,record_amount,record_cost")
            print("示例基金，1000,1,1000,1")
            print("招商产业债，9768.82,1.8324,8466.3,1.8305")
            return

        try:
            with open(csv_path, 'r', encoding='utf-8') as f:
                reader = csv.DictReader(f)

                # 检查必需的列
                required_columns = ['name', 'actual_amount', 'actual_cost',
                                   'record_amount', 'record_cost']
                if not all(col in reader.fieldnames for col in required_columns):
                    print(f"错误：CSV 文件缺少必需的列")
                    print(f"需要的列：{', '.join(required_columns)}")
                    print(f"当前的列：{', '.join(reader.fieldnames)}")
                    return

                # 读取数据
                for row in reader:
                    name = row['name'].strip()
                    if not name:  # 跳过空行
                        continue

                    try:
                        self.funds[name] = {
                            "actual_amount": Decimal(row['actual_amount'].strip()),
                            "actual_cost": Decimal(row['actual_cost'].strip()),
                            "record_amount": Decimal(row['record_amount'].strip()),
                            "record_cost": Decimal(row['record_cost'].strip()),
                        }
                    except (ValueError, KeyError) as e:
                        print(f"警告：跳过无效数据行 - {name}: {e}")
                        continue

                print(f"成功从 {csv_path} 加载 {len(self.funds)} 个基金")
                print()

        except Exception as e:
            print(f"错误：读取 CSV 文件失败：{e}")

    def calculate_adjustment(
        self,
        actual_amount: Decimal,
        actual_cost: Decimal,
        record_amount: Decimal,
        record_cost: Decimal
    ) -> Tuple[str, Decimal, Decimal]:
        """
        计算需要的修正交易

        返回：(操作类型，数量，价格)
        - 操作类型："买入" 或 "卖出"
        - 数量：需要交易的份额
        - 价格：交易价格
        """
        # 计算实际总成本
        actual_total_cost = actual_amount * actual_cost
        # 计算记账工具中的总成本
        record_total_cost = record_amount * record_cost

        # 计算数量差异
        amount_diff = actual_amount - record_amount

        if amount_diff == 0:
            # 数量相同，只需调整成本
            if actual_total_cost == record_total_cost:
                return ("无需调整", Decimal("0"), Decimal("0"))
            else:
                # 通过买入/卖出 0 份额但不同价格来调整成本（实际操作中可能需要其他方式）
                # 这里采用买入少量份额的方式来调整总成本
                adjust_amount = Decimal("0.0001")
                cost_diff = actual_total_cost - record_total_cost
                adjust_price = cost_diff / adjust_amount
                return ("买入 (调整成本)", adjust_amount, adjust_price)

        elif amount_diff > 0:
            # 实际持有更多，需要"买入"
            # 计算买入价格：使得买入后总成本和数量都正确
            # 新总成本 = 记账总成本 + 买入数量 * 买入价格
            # actual_total_cost = record_total_cost + amount_diff * buy_price
            buy_price = (actual_total_cost - record_total_cost) / amount_diff
            return ("买入", amount_diff, buy_price)

        else:  # amount_diff < 0
            # 实际持有更少，需要"卖出"
            sell_amount = -amount_diff
            # 计算卖出价格：使得卖出后总成本和数量都正确
            # 卖出后剩余总成本 = actual_total_cost
            # 卖出前总成本 - 卖出数量 * 卖出成本 = actual_total_cost
            # 这里假设按记账成本卖出
            sell_price = (record_total_cost - actual_total_cost) / sell_amount
            return ("卖出", sell_amount, sell_price)

    def format_decimal(self, value: Decimal) -> str:
        """格式化为 4 位小数"""
        return str(value.quantize(Decimal("0.0001"), rounding=ROUND_HALF_UP))

    def process_all_funds(self):
        """处理所有基金，输出修正建议"""
        if not self.funds:
            print("没有可处理的基金数据")
            return

        print("=" * 80)
        print("基金持仓修正报告")
        print("=" * 80)
        print()

        for fund_name, data in self.funds.items():
            print(f"[{fund_name}]")
            print(f"  实际持仓：数量={self.format_decimal(data['actual_amount'])}, "
                  f"成本={self.format_decimal(data['actual_cost'])}, "
                      f"总额={self.format_decimal(data['actual_amount'] * data['actual_cost'])}")
            print(f"  记账数据：数量={self.format_decimal(data['record_amount'])}, "
                  f"成本={self.format_decimal(data['record_cost'])}, "
                      f"总额={self.format_decimal(data['record_amount'] * data['record_cost'])}")

            action, amount, price = self.calculate_adjustment(
                data['actual_amount'],
                data['actual_cost'],
                data['record_amount'],
                data['record_cost']
            )

            if action == "无需调整":
                print(f"  ✓ 无需调整")
            else:
                print(f"  → 修正操作：{action}")
                print(f"     数量：{self.format_decimal(amount)}")
                print(f"     价格：{self.format_decimal(price)}")

                # 验证修正后的结果
                if action == "买入" or action == "买入 (调整成本)":
                    new_amount = data['record_amount'] + amount
                    new_total_cost = (data['record_amount'] * data['record_cost'] +
                                     amount * price)
                else:  # 卖出
                    new_amount = data['record_amount'] - amount
                    new_total_cost = (data['record_amount'] * data['record_cost'] -
                                     amount * price)

                new_cost = new_total_cost / new_amount if new_amount != 0 else Decimal("0")
                print(f"     修正后：数量={self.format_decimal(new_amount)}, "
                      f"成本={self.format_decimal(new_cost)}, "
                      f"总额={self.format_decimal(new_amount * new_cost)}")

            print()

        print("=" * 80)

    def add_fund(self, name: str, actual_amount: float, actual_cost: float,
                 record_amount: float, record_cost: float):
        """添加新的基金数据"""
        self.funds[name] = {
            "actual_amount": Decimal(str(actual_amount)),
            "actual_cost": Decimal(str(actual_cost)),
            "record_amount": Decimal(str(record_amount)),
            "record_cost": Decimal(str(record_cost)),
        }

def main():
    # 从 CSV 文件加载基金数据
    tool = FundAdjustmentTool("funds.csv")

    # 如果需要，也可以手动添加基金
    # tool.add_fund("基金名称", 实际数量，实际成本，记账数量，记账成本)
    # 例如：
    # tool.add_fund("招商中证白酒", 1200.5000, 1.8500, 1000.0000, 1.8000)

    # 处理所有基金并输出报告
    tool.process_all_funds()


if __name__ == "__main__":
    main()
