from survey import AnonymousSurvey

question = "你的母语是什么？"
my_survey = AnonymousSurvey(question)

my_survey.show_question()
print("输入'q'退出")
while True:
    response = input("语言：")
    if response == 'q':
        break
    my_survey.store_response(response)

my_survey.show_results()