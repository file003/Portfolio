/**
* Assignment 5: Page replacement algorithms
 * @file lru_replacement.cpp
 * @author David File
 * @brief A class implementing the LRU page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lru_replacement.h"
#include <iostream>

// TODO: Add your implementation here
LRUReplacement::LRUReplacement(int num_pages, int num_frames)
  : Replacement(num_pages, num_frames)
{
  // TODO: Complete this constructor
}

// TODO: Add your implementations for desctructor, touch_page, load_page, replace_page here
LRUReplacement::~LRUReplacement()
{
  page_list.clear();
  page_map.clear();
}

// Accesss a page alreay in physical memory
void LRUReplacement::touch_page(int page_num)
{
  //Update the page access order
  auto it = page_map.find(page_num);
  if (it != page_map.end()) {
    //Page exists in page map
    page_list.erase(it->second);
    page_list.push_back(page_num);
    it->second = std::prev(page_list.end());
  }
}

// Access an invalid page, but free frames are available
void LRUReplacement::load_page(int page_num) {
  //Change state of page
  page_table[page_num].valid = true;
  //Assign page to available frame and update
  page_list.push_back(page_num);
  page_map[page_num] = std::prev(page_list.end());
}

// Access an invalid page and no free frames are available
int LRUReplacement::replace_page(int page_num) {
  //Replace least recently used page and update
  int victim_page = page_list.front();
  page_list.pop_front();
  page_map.erase(victim_page);
  page_table[victim_page].valid = false;
  page_table[page_num].valid = true;
  page_list.push_back(page_num);
  page_map[page_num] = std::prev(page_list.end());
  
  return 0;
}