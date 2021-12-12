[TOC]

openpyxl
---

# 作用
读写`xlsx`文件。

# 安装
```
pip install openpyxl
```

# 读取
```python
import openpyxl

filename = "xxx.xlsx"

# 加载文档
workbook = openpyxl.load_workbook(filename)

# 获取worksheet
worksheet = wb.get_sheet_by_name("Sheet1")  # worksheet = workbook.active

# 获取单元格：第一行第一列
cell = sheet["A1"]

# 获取单元格的值
value = cell.value
```


# 写入
```python
import openpyxl

# 创建对象
workbook = openpyxl.Workbook()

# 获取当前活跃的worksheet,默认就是第一个worksheet
worksheet = workbook.active

# 设置标题
worksheet.title = "Sheet1"

# 设置row行，col列的值为value
row = 1
col = 2
worksheet.cell(row, col, "value")

# 设置列宽
col = "A"
worksheet.column_dimensions[col].width = sheet.column_dimensions[col].width # 20

# 设置行高
row = 1
worksheet.row_dimensions[row].height = sheet.row_dimensions[row].height    # 20

# 获取单元格: (1,1)
cell = worksheet["A1"]

# 设置单元格背景颜色等
fill = PatternFill(start_color=othercell.fill.start_color,  # FFFFFF00: 黄色
                end_color=othercell.fill.start_color,
                fill_type='solid')
cell.fill = fill

# 保存文件
filename = "xxx.xlsx"
workbook.save(filename=filename)
```