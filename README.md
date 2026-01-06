# Sand Simulator

This is a sand simulator with **1.2 million cells**, which runs at **160FPS** on **old hardware**.

### Details of the previous statement
- The simulation happens in a 1536x768 grid, which totals to 1179648 cells.
- The simulation is *single-threaded*. I did try to make it multithreaded, but all my models either didn't work,
broke sand/water's logic, or dropped my FPS to straight up 20.
- The framerate is obviously jumping up and down during execution, depending on load. 160 is just a rough average.
- The diagnostics function(GetFPS(), basically) *visibly* slows down the simulation, so the actual framerate
is higher than 160.
- The FPS of 160 was recorded *specifically* when I was *actively* filling roughly a half of the screen with both sand
and water. As soon as I stop adding anything for about 2 seconds, FPS jacks to 200+.
- These FPS were recorded with "-DNDEBUG", which disables all the asserts I used. This flag is not present in the
Makefile, so be aware if you want to fact-check my claim of FPS.

### The hardware used:
- i5 7500 (4C/4T, 3.4-3.8 GHz);
- iGPU/no dedicated GPU;
- 8GB of DDR4 RAM(2400MT/s);

# Previous attempts
This is the third attempt(or second rewrite) of this project. The first attempt was in C with some C++ features, 
the second was in Odin. This version is in pure C, with no C++ features used.

Both previous attempts are private repos.

# AI?
**NO AI WAS USED TO WRITE THE CODE OF THIS PROJECT. AI WILL NOT BE USED TO WRITE CODE FOR _ANY_ OF MY PROJECTS. EVER.**
The only AI used, was to catch off-by-one and other typo bugs in my code, which I am awful at seeing.

# Ruleset
The rules are very simple. Sand can move straight down or diagonally down, in that order of preference, and it 
displaces water. Water can move down, diagonally down or sideways, in that order of preference, and it cannot displace 
anything.

# Optimizations
I will note the main/most impactful ones.
- Each chunk goes dead after 5 ticks of inactivity;
- If there were no writes on a given chunk during a frame, it does not get written to screen buffer;
- Each chunk is a continuous piece of memory;
- Chunk size is 8x8, and it consists of u8s. This makes it so every chunk is exactly 64 bytes big, and fits into
a single cache line;
- The chunks are all aligned to 64 bytes(though doing this one didn't have much effect. gcc was probably aligning them
even wihtout my interference);
- The divisions and modulo operations in the get_cell function are replaced for bit shifts and bitwise ANDs. I
ensured that the chunk size is *always* a power of two, using static asserts. *Yes*, they were impacting the
performance enough to be worried about. Perf said so. *idiv*s are *evil*.
- All the changes are written into a buffer in CPU memory during the frame, and, at the end of the frame, sent to GPU
memory all at once. This is basically software rendering, but it's *faster* when you do tons of per-pixel updates. GPU
*struggles* with  draw calls per frame, even if batched.

# Concerning multithreading
As I said, I *did* try multithreading, but it didn't work out for me. At all. The amount of dependencies cells have on 
each other is just too great. Utilising 4 threads(which is what I have) for this work requires such frequent 
coordination, that single-threaded version just becomes 12-13x faster, straight up.
The best model I have on paper, requires 256 check-ins from each thread *per frame*, which is obviously bonkers.
The model with almost no check-ins that I have, breaks the water and sand physics at the seams of quarters of the grid. 

It's just not doable, at least with my ruleset of sand-water physics.
For reference, in this model, each chunk depends on chunk to the left, and all 3 chunks below. They must be all 
finished before the given chunk can be processed.

This is probably much more doable with double-buffering of the grid, but my attempts to implement double-buffering 
resulted in about 12 kinds of different bugs, including:
- sand getting deleted;
- water getting duplicated;
- water dispersing into fine mist;
- water randomly moving to the right and not falling;
- water falling and *then* moving all to the right;
- water falling to the ground, but not really settling, and having "trapped air";
- sand having a bias to the right that I could not fix whatever I do;
- other bugs.

So, I ditched the idea of doing double-buffering. Maybe you can do it and get it right.
