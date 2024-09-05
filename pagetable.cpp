/**
* Assignment 5: Page replacement algorithms
 * @file pagetable.cpp
 * @author David File
 * @brief This class represents a traditional pagetable data structure.
 * @version 0.1
 */
//You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include "pagetable.h"

// TODO: Add your implementation of PageTable
// Constrcutor
// TODO: Add your code
PageTable::PageTable(int num_pages) {
  pages.resize(num_pages);
  // Initialize the page table entries
  for (int i = 0; i < num_pages; i++) {
    pages[i].valid = false;
  }
}

// Destructor
// TODO: Add your code
PageTable::~PageTable() {}

/**
 * @brief Check if a page is present in the physical memory.
 * @param i The index of the page.
 * @return True if the page is valid (present in physical memory), false
 * otherwise.
 */
bool PageTable::isPageValid(int i) { return pages[i].valid; }

/**
 * @brief Mark a page as valid (present in physical memory).
 * @param i The index of the page.
 */
void PageTable::markPageValid(int i) { pages[i].valid = true; }