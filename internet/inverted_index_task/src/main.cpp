#include <iostream>
#include <fstream>
#include "pugixml.hpp"

int main() {
  // Загружаем HTML-код из файла
  std::ifstream file("/home/gennadiy/third_course/internet/inverted_index_task/collection_html/All's Well That Ends Well  Entire Play.htm");
  std::string html((std::istreambuf_iterator<char>(file)),
                    std::istreambuf_iterator<char>());

  // Создаем объект xml_document и загружаем HTML-код
  pugi::xml_document doc;
  doc.load_string(html.c_str());

  // Получаем заголовок пьесы
  pugi::xml_node title = doc.child("html").child("head").child("title");
  std::cout << "Title: " << title.text().as_string() << std::endl;

  // Получаем текст первого реплики
  pugi::xml_node first_speech = doc.child("html").child("body").child("table").child("tr").child("td").child("H3").next_sibling().child("p").child("blockquote").child("A");
  std::cout << "First speech: " << first_speech.text().as_string() << std::endl;

  // Выводим все реплики 
  for (auto const& speech : doc.select_nodes("//A")) {
    std::cout << "Speaker: " << speech.attribute().as_string() << std::endl; 
  }

  return 0;
}

