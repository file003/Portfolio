/**
* Assignment 5: Page replacement algorithms
 * @file replacement.cpp
 * @author David File
 * @brief A base class for different page replacement algorithms.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code
#include <iostream>
#include "replacement.h"

// TODO: Add your implementation of the Replacement member functions here

// Constructor
Replacement::Replacement(int num_pages, int num_frames)
: page_table(num_pages)
{
  this->num_frames = num_frames;
  this->num_pages = num_pages;
  num_page_faults = 0;
  num_page_replacements = 0;
  frameNum = 0;
  refNum = 0;
  numFreeFrames = num_frames;
}

// Destructor
Replacement::~Replacement()
{
    // TOOD: Add your code here
}

// Simulate a single page access 
// @return true if it's a page fault
bool Replacement::access_page(int page_num, bool is_write)
{
  // If the page is valid, it calls the touch_page function. 
  if (page_table[page_num].valid) {
    touch_page(page_num);
    refNum++;
    return false;
  }
  
  // If the page is not valid but free frames are available, it calls the load_page function.
  if (!page_table[page_num].valid && numFreeFrames > 0) {
    load_page(page_num);
    page_table[page_num].frame_num = frameNum;
    //page_table.markPageValid(page_num);
    frameNum ++;
    refNum++;
    num_page_faults++;
    numFreeFrames--;
    return true;
  }
  
  // If the page is not valid and there is no free frame, it calls the replace_page function.
  else if(!page_table[page_num].valid && numFreeFrames == 0){
    int tempFrameNum = replace_page(page_num);
    page_table[page_num].frame_num = tempFrameNum;
    refNum++;
    num_page_faults++;
    num_page_replacements++;
    return true;
  }
  
  return false;
}

// Print out statistics of simulation
void Replacement::print_statistics() const {
        // TODO: print out the number of references, number of page faults and number of page replacements
		std::cout << "Number of references: \t\t"  << std::endl;
		std::cout << "Number of page faults: \t\t" << std::endl;
		std::cout << "Number of page replacements: \t"  << std::endl;
}