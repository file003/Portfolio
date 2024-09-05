/**
* Assignment 5: Page replacement algorithms
 * @file lifo_replacement.cpp
 * @author David FIle
 * @brief A class implementing the Last in First Out (LIFO) page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "lifo_replacement.h"

// TODO: Add your implementation here
LIFOReplacement::LIFOReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames)
{
    // nothing is needed
}

// TODO: Add your implementations for desctructor, load_page, replace_page here
LIFOReplacement::~LIFOReplacement() {
    // nothing is needed
}

// Access an invalid page, but free frames are available
void LIFOReplacement::load_page(int page_num) {
  //Assign page to available frame
  page_table[page_num].valid = true;
  frame_stack.push(page_num); //Add frame index to queue 
}

// Access an invalid page and no free frames are available
int LIFOReplacement::replace_page(int page_num) {
  //Get index of oldest page
  int victim_page = frame_stack.top();
  frame_stack.pop(); //Remove oldest page from queue
  // Replace page with new page
  page_table[page_num].valid = true;
  page_table[victim_page].valid = false;
  //Add new page to end of queue
  frame_stack.push(page_num);     
  
  return 0;
}