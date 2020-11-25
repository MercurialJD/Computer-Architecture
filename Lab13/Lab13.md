## Exercise 1

1. Given the way the address was broken down, how big are the pages in this model?

   **32 words**

2. How many TLB Hits and Misses did we have for the randomly generated set of ten addresses? What about for Page Hits and Page Faults?

   **2 8 0 8**

3. Did you have any Page Hits? Explain why. Can you think of a set of ten addresses that would result in a Page Hit?

   **No! The number of physical memory frames is equal to the number of TLB entries. If TLB Hit, then the Page Table won't be visited; If TLB Miss, definitely no Page Hit. No I can't.**

4. Explain the process by which we turn a virtual address into a physical address for the very first access (emphasizing on TLB Misses and Page Faults).

   **Split the address bits -> Use the page fields to find the page -> Check TLB: miss -> Check Page Table: fault -> take data from disk to the first available frame (0) of physical memory -> Write TLB and Page Table -> load data**



## Exercise 2

1. Demonstrate that your ten memory accesses results in ten TLB Misses and ten Page Faults. Explain why such behavior occurs.

   **`00 20 40 60 80 00 20 40 60 80`**

   **After splitting the address bits, the page fields are always new to the TLB (000, 001, 010, 011, 100...) so always TLB Miss. Due to the same reason to Exercise 1, always Page Faults.**



## Exercise 3

1. Explain the single parameter change that would result in ten TLB misses, but **fewer** than ten page faults.

   **Increase the physical memory size.**

