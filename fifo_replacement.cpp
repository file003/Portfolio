/**
* Assignment 5: Page replacement algorithms
 * @file fifo_replacement.cpp
 * @author David File
 * @brief A class implementing the FIFO page replacement algorithms
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "fifo_replacement.h"

// TODO: Add your implementation here
FIFOReplacement::FIFOReplacement(int num_pages, int num_frames)
: Replacement(num_pages, num_frames)
{
}

// TODO: Add your implementations for desctructor, load_page, replace_page here
FIFOReplacement::~FIFOReplacement() {
}

// Access an invalid page, but free frames are available
void FIFOReplacement::load_page(int page_num) {
  page_table[page_num].valid = true; //Assign the page to available frame
  frame_queue.push(page_num); //Add frame index to queue
}

// Access an invalid page and no free frames are available
int FIFOReplacement::replace_page(int page_num) {
  int victim_page = frame_queue.front(); //Get the index of oldest page
  frame_queue.pop(); //Remove oldest page from queue
  //Replace page with new page
  page_table[page_num].valid = true;
  page_table[victim_page].valid = false;
  frame_queue.push(page_num); //Add new page to end of queue

  return 0;
}