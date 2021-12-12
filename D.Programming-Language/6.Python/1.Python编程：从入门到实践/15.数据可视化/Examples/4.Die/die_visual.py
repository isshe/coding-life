from die import Die
import pygal

die = Die()
die_2 = Die()

results = []
for roll_num in range(1000):
    result = die.roll() + die_2.roll()
    results.append(result)

print(results)

# 分析结果
frequencies = []
max_result = die.num_sides + die_2.num_sides
for value in range(1, max_result + 1):
    frequency = results.count(value)
    frequencies.append(frequency)

print(frequencies)

# 对结果进行可视化
hist = pygal.Bar()
hist.title = "Results of rollinig two D6 1000 time"
hist.x_labels = ['2', '3', '4', '5', '6', '7', '8', '9', '10', '11', '12']
hist.x_title = "Result"
hist.y_title = "Frequency of Result"

hist.add("D6", frequencies)
hist.render_to_file('die_visual.svg')