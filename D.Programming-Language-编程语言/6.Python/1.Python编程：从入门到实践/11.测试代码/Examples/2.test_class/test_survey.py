import unittest
from survey import AnonymousSurvey

class TestAnonymousSurvey(unittest.TestCase):
    def setUp(self):
        self.response = "Chinese"

    def test_store_single_response(self):
        question = "你的母语是什么？"
        my_survey = AnonymousSurvey(question)
        my_survey.store_response(self.response)

        self.assertIn(self.response, my_survey.responses)

unittest.main()