#pragma once

#include <fstream>
#include <iostream>

#include "SStock.h"
#include "CList.h"

class CDataBase
{
	CList<SStock> dataBase;
public:
	void buildFromFile(const std::string filePath)
	{
		int i{ 0 };
		std::string line;
		std::ifstream objFile(filePath);
		if (objFile.is_open())
		{
			// Skip the first line
			std::getline(objFile, line);

			// Process the rest of the file
			while (std::getline(objFile, line))
			{
				const std::string separatorCsv{ "," };
				const std::string separatorDate{ "-" };

				std::size_t a = line.find_first_of(separatorCsv);
				std::size_t b = line.find_first_of(separatorCsv, a + 1);
				std::size_t c = line.find_first_of(separatorCsv, b + 1);
				std::size_t d = line.find_first_of(separatorCsv, c + 1);
				std::size_t e = line.find_first_of(separatorCsv, d + 1);

				std::string date = line.substr(0, a);
				std::string open = line.substr(a + 1, b - a - 1);
				std::string max = line.substr(b + 1, c - b - 1);
				std::string min = line.substr(c + 1, d - c - 1);
				std::string close = line.substr(d + 1, e - d - 1);
				std::string volume = line.substr(e + 1, line.size() - e - 1);

				std::size_t da = date.find_first_of(separatorDate);
				std::size_t db = date.find_first_of(separatorDate, da + 1);

				std::string year = date.substr(0, da);
				std::string month = date.substr(da + 1, db - da - 1);
				std::string day = date.substr(db + 1, date.size() - db - 1);

				SStock* temp = new SStock();
				temp->date.day = std::stoi(day);
				temp->date.month = std::stoi(month);
				temp->date.year = std::stoi(year);
				temp->open = std::stod(open);
				temp->max = std::stod(max);
				temp->min = std::stod(min);
				temp->close = std::stod(close);
				temp->volume = std::stod(volume);
				dataBase.push_back(temp);
			}

			objFile.close();
			std::cout << "DataBase contains " << dataBase.numOfItems << " records." << std::endl;
		}
	}
	SStock* at(int i)
	{
		return dataBase.at(i);
	}
	unsigned int size()
	{
		return dataBase.numOfItems;
	}
};