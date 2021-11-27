## Prepare 
### Clone this repository  
```shell
git clone https://github.com/DYGV/task_scheduler.git
cd task_scheduler/
```
<details>
<summary>Directory Tree</summary>

```
.
├── Doxyfile
├── Makefile
├── README.md
└── scheduler
    ├── api
    │   ├── include
    │   │   └── sem.h
    │   └── sem.c
    ├── internal
    │   ├── include
    │   │   ├── queue.h
    │   │   ├── schedule.h
    │   │   └── tcb.h
    │   ├── queue.c
    │   ├── schedule.c
    │   └── tcb.c
    ├── multi_sem.c
    ├── multi_sem_idle.c
    ├── single_sem.c
    └── single_sem_idle.c
```
</details>

## Build
### Use make command  
```shell
make all
```
The generated executable is as follows:
- `./bin/single_sem`
- `./bin/single_sem_idle`
- `./bin/multi_sem`
-  `./bin/multi_sem_idle`
## Documents
[Doxygen](https://www.doxygen.nl/download.html#gitrepos) must be installed on your computer to document.
```shell
doxygen
```
#### Optional packages
- [Graphviz](https://graphviz.org/download/)
