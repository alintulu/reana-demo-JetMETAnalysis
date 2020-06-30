all: RunListRunLumi RunMatchFiles RunPrepareStep2Submission Columnize

RunListRunLumi: ListRunLumi.cpp
	g++ ListRunLumi.cpp -o RunListRunLumi \
		`root-config --cflags --libs`

RunMatchFiles: MatchFiles.cpp
	g++ MatchFiles.cpp -o RunMatchFiles

RunPrepareStep2Submission: PrepareStep2Submission.cpp
	g++ PrepareStep2Submission.cpp -o RunPrepareStep2Submission

Columnize: Columnize.cpp
	g++ Columnize.cpp -o Columnize



