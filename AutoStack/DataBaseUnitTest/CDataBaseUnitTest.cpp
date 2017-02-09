#include "stdafx.h"
#include "CppUnitTest.h"
#include "../AutoStack/CDataBase.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DataBaseUnitTest
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		CDataBase dataBase;
		TEST_METHOD(VerifyFirstDataItemDate)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(0);
			Assert::AreEqual(static_cast<unsigned int>(1991), result->date.year);
			Assert::AreEqual(static_cast<unsigned int>(4), result->date.month);
			Assert::AreEqual(static_cast<unsigned int>(16), result->date.day);
		}
		TEST_METHOD(VarifyFirstDataItemContent)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(0);
			Assert::AreEqual(static_cast<double>(1000), result->open);
			Assert::AreEqual(static_cast<double>(1000), result->max);
			Assert::AreEqual(static_cast<double>(1000), result->min);
			Assert::AreEqual(static_cast<double>(1000), result->close);
			Assert::AreEqual(static_cast<double>(325), result->volume);
		}
		TEST_METHOD(VerifySecondDataItemDate)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(1);
			Assert::AreEqual(static_cast<unsigned int>(1991), result->date.year);
			Assert::AreEqual(static_cast<unsigned int>(4), result->date.month);
			Assert::AreEqual(static_cast<unsigned int>(23), result->date.day);
		}
		TEST_METHOD(VarifySecondDataItemContent)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(1);
			Assert::AreEqual(static_cast<double>(967.7), result->open);
			Assert::AreEqual(static_cast<double>(967.7), result->max);
			Assert::AreEqual(static_cast<double>(967.7), result->min);
			Assert::AreEqual(static_cast<double>(967.7), result->close);
			Assert::AreEqual(static_cast<double>(5905), result->volume);
		}
		TEST_METHOD(VerifyTenthDataItemDate)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(10);
			Assert::AreEqual(static_cast<unsigned int>(1991), result->date.year);
			Assert::AreEqual(static_cast<unsigned int>(7), result->date.month);
			Assert::AreEqual(static_cast<unsigned int>(2), result->date.day);
		}
		TEST_METHOD(VarifyTenthDataItemContent)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(10);
			Assert::AreEqual(static_cast<double>(891.2), result->open);
			Assert::AreEqual(static_cast<double>(891.2), result->max);
			Assert::AreEqual(static_cast<double>(891.2), result->min);
			Assert::AreEqual(static_cast<double>(891.2), result->close);
			Assert::AreEqual(static_cast<double>(31913), result->volume);
		}
		TEST_METHOD(VarifyNumberOfItems)
		{
			dataBase.buildFromFile("wig_d.csv");
			Assert::AreEqual(static_cast<unsigned int>(6009), dataBase.size());
		}
		TEST_METHOD(VerifyLastDataItemDate)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(6008);
			Assert::AreEqual(static_cast<unsigned int>(2017), result->date.year);
			Assert::AreEqual(static_cast<unsigned int>(2), result->date.month);
			Assert::AreEqual(static_cast<unsigned int>(7), result->date.day);
		}
		TEST_METHOD(VarifyLastDataItemContent)
		{
			dataBase.buildFromFile("wig_d.csv");
			SStock* result = dataBase.at(6008);
			Assert::AreEqual(static_cast<double>(55606.83), result->open);
			Assert::AreEqual(static_cast<double>(55758.32), result->max);
			Assert::AreEqual(static_cast<double>(55457.09), result->min);
			Assert::AreEqual(static_cast<double>(55617.09), result->close);
			Assert::AreEqual(static_cast<double>(36911846), result->volume);
		}
	};
}