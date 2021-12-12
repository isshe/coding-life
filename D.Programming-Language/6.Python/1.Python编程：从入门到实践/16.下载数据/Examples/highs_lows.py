import csv

from matplotlib import pyplot as plt
from datetime import datetime


def a():
    filename = "sitka_weather_07-2018_simple.csv"
    with open(filename) as f:
        reader = csv.reader(f)
        header_row = next(reader)
        print(header_row)

        for index, column_header in enumerate(header_row):
            print(index, column_header)

        highs = []
        dates = []
        for row in reader:
            date = datetime.strptime(row[2], '%Y-%m-%d')
            dates.append(date)
            highs.append(int(row[5]))

        print(dates)
        print(highs)

        # 根据数据绘制图形
        fig = plt.figure(dpi=64, figsize=(10, 6))
        plt.plot(dates, highs, c='red')

        # 设置图形的格式
        plt.title("Daily high temperatures, July 2018", fontsize=14)
        plt.xlabel('', fontsize=16)
        fig.autofmt_xdate()  # ???
        plt.ylabel("Temmperature(F)", fontsize=16)
        plt.tick_params(axis='both', which='major', labelsize=16)

        plt.show()


def b():
    filename = "sitka_weather_2014.csv"
    with open(filename) as f:
        reader = csv.reader(f)
        header_row = next(reader)
        print(header_row)

        for index, column_header in enumerate(header_row):
            print(index, column_header)

        highs = []
        dates = []
        lows = []
        for row in reader:
            try:
                date = datetime.strptime(row[0], '%Y-%m-%d')
                high = int(row[1])
                low = int(row[3])
            except ValueError:
                print(date, 'missing data')
            else:
                dates.append(date)
                highs.append(high)
                lows.append(low)

        print(dates)
        print(highs)

        # 根据数据绘制图形
        fig = plt.figure(dpi=64, figsize=(10, 6))
        plt.plot(dates, highs, c='red')
        plt.plot(dates, lows, c='blue')

        # 区域着色
        plt.fill_between(dates, highs, lows, facecolor='blue', alpha=0.2)

        # 设置图形的格式
        plt.title("Daily high/low temperatures - 2014", fontsize=24)
        plt.xlabel('', fontsize=16)
        fig.autofmt_xdate()  # ???
        plt.ylabel("Temmperature(F)", fontsize=16)
        plt.tick_params(axis='both', which='major', labelsize=16)

        plt.show()


if __name__ == "__main__":
    #a()
    b()
