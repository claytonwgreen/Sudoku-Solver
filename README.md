# Sudoku Solver

Can either solve complete puzzle of give you a hint by giving you the number that belongs in a specific square

---

### Build
```bash
make
```

---

### Run
```bash
./solve
```

- Follow prompts for inputting the puzzle and chosing to solve or just get a hint

---

### Logic
- This program is designed to be a mix efficiency and power
	* It has simple logic that attempts to solve the puzzle in essentially the same way a person would (cross-hatching) 
		* This logic is run before the more complex, as it will run faster
		* For simple puzzles, this logic is enough to solve the puzzle, and does so faster than any sort of DFS or BFS
	* It also has logic for a DFS which can solve any proper sudoku puzzle
		* This logic takes longer as it essentially guesses and checks an incredibly large number of possibilities 
		* Even if the simple logic that runs first is unable to completely solve the puzzle, it will often get a partial solution. This partial solution is extremely helpful for the DFS as a single additional known value can theoretically reduce the time taken to solve the puzzle 10 fold



DETECTOR
- brew install pkg-config
- brew install opencv@2
- brew will print out commands for setting env variables to allow use of opencv
	- this will allow compiler to find opencv (ex: export PKG_CONFIG_PATH="/usr/local/opt/opencv@2/lib/pkgconfig")