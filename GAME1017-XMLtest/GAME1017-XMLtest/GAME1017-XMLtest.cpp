#include <vector>
#include <string>
#include "tinyxml2.h"


int main()
{
	tinyxml2::XMLDocument xmlDoc; // Object of the DOM. Tree.
	tinyxml2::XMLNode* pRoot = xmlDoc.NewElement("Root"); // Bookmark of the root of the tree.
	xmlDoc.InsertFirstChild(pRoot);

	// Think of the elements as "bookmarks" that help you find your saved values in the XML doc.
	// Adding PlayerID
	tinyxml2::XMLElement* pElement = xmlDoc.NewElement("PlayerID");
	pElement->SetText(39428372);
	pRoot->InsertEndChild(pElement);
	// Adding Player HP
	pElement = xmlDoc.NewElement("Health");
	pElement->SetText(100);
	pRoot->InsertEndChild(pElement);
	// Adding Player Damage
	pElement = xmlDoc.NewElement("Damage");
	pElement->SetText(15);
	pRoot->InsertEndChild(pElement);


	// 1. Making the date with multiple variables in one bookmark.
	pElement = xmlDoc.NewElement("Date");
	pElement->SetAttribute("day", 27);
	pElement->SetAttribute("month", "January");
	pElement->SetAttribute("year", 2023);
	pRoot->InsertAfterChild(pRoot->FirstChildElement("Health"), pElement);
	// 2. Making a list of integers and labelling each one as an "Item".
	pElement = xmlDoc.NewElement("List");
	std::vector<int> vecList = {5, 2, 83, 90, 103, 32};
	tinyxml2::XMLElement* pListElement;
	for (const auto& i : vecList) // For each integer in the vector
	{
		pListElement = xmlDoc.NewElement("Item");
		pListElement->SetText(i);
		pElement->InsertEndChild(pListElement);
	}
	pElement->SetAttribute("count", vecList.size()); // Helps keep storage dynamic :D

	// Saving the file and then ending the program...
	xmlDoc.SaveFile("SaveFile.xml");
	return 0;
}
