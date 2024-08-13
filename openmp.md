
### Common Directives and Clauses (Continued)
Common Directives and Clauses
1. **#pragma omp parallel**: Defines a parallel region.
2. **#pragma omp for**: Distributes loop iterations across threads.
3. **`#pragma omp sections`**: Divides work among threads with different code blocks.
4. **`#pragma omp single`**: Ensures that a block of code is executed by only one thread.
5. **`#pragma omp master`**: Restricts execution to the master thread.
6. **`#pragma omp critical`**: Defines a critical section that only one thread can execute at a time.
7. **`#pragma omp barrier`**: Synchronizes all threads in a team.
8. **`#pragma omp atomic`**: Ensures atomic operations on a variable.
9. **`#pragma omp task`**: Defines a task to be executed by a thread.
10. **`#pragma omp taskwait`**: Waits for the completion of child tasks.
11. **`#pragma omp taskgroup`**: Groups tasks for collective waiting.
12. **`#pragma omp flush`**: Ensures memory consistency across threads.
13. **`#pragma omp threadprivate`**: Declares global variables to be private to each thread.
14. **`#pragma omp ordered`**: Enforces sequential execution of a code block within a loop.
15. **`#pragma omp simd`**: Enables SIMD parallelism for loops.
16. **`#pragma omp cancel`**: Cancels a parallel region, loop, or task.
17. **`#pragma omp cancellation point`**: Defines a point where cancellation is checked.
18. **`#pragma omp target`**: Offloads execution to a target device like a GPU.
19. **`#pragma omp teams`**: Creates teams of threads on a target device.

### Additional Directives and Clauses

20. **`#pragma omp distribute`**: Distributes iterations of the loop across teams.
21. **`#pragma omp declare simd`**: Specifies that a function can be vectorized using SIMD.
22. **`#pragma omp declare target`**: Marks a function or variable for execution on a target device.
23. **`#pragma omp end declare target`**: Ends the region marked for execution on a target device.
24. **`#pragma omp distribute parallel for`**: Combines distribute and parallel for to distribute loops across teams.
25. **`#pragma omp distribute simd`**: Distributes iterations and applies SIMD parallelism.
26. **`#pragma omp taskloop`**: Distributes iterations of a loop among tasks.
27. **`#pragma omp taskloop simd`**: Combines taskloop and SIMD to parallelize loops with tasks and vectorization.
28. **`#pragma omp target data`**: Maps data to/from a target device.
29. **`#pragma omp target enter data`**: Maps data to a target device without execution.
30. **`#pragma omp target exit data`**: Removes data mapping from a target device.
31. **`#pragma omp target update`**: Updates data between the host and target device.
32. **`#pragma omp teams distribute`**: Distributes loops across teams on a target device.
33. **`#pragma omp teams distribute parallel for simd`**: Combines several constructs to parallelize loops across teams with SIMD on a target device.
34. **`#pragma omp parallel for simd`**: Combines parallel for and SIMD directives.
35. **`#pragma omp declare reduction`**: Defines a user-defined reduction operation.
36. **`#pragma omp declare mapper`**: Defines a user-defined mapping for target devices.
37. **`#pragma omp allocate`**: Allocates memory with specified attributes.
38. **`#pragma omp requires`**: Specifies requirements for the execution environment (e.g., unified shared memory).
39. **`#pragma omp metadirective`**: Allows for compile-time selection of directives based on the context.
40. **`#pragma omp scan`**: Performs a prefix scan operation.

### Clauses Used with Directives

- **`shared`**: Specifies that variables are shared among all threads.
- **`private`**: Each thread has its own copy of the variable.
- **`firstprivate`**: Like `private`, but the initial value is copied from the master thread.
- **`lastprivate`**: Updates the original variable with the last iteration’s value.
- **`reduction`**: Combines variables from all threads into a single result.
- **`nowait`**: Omits the implicit barrier at the end of work-sharing constructs.
- **`schedule`**: Controls the distribution of loop iterations (e.g., `static`, `dynamic`, `guided`).
- **`collapse`**: Collapses nested loops into a single loop for parallelization.
- **`default`**: Sets the default data-sharing attribute (e.g., `shared`, `none`).
- **`if`**: Specifies a condition under which parallel execution occurs.
- **`num_threads`**: Sets the number of threads in a parallel region.
- **`copyin`**: Copies the value of a thread-private variable from the master thread.
- **`proc_bind`**: Specifies how threads are bound to processors.
