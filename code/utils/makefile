all: RunListRunLumi RunMatchFiles RunPrepareMatching

RunListRunLumi: ListRunLumi.cpp
	g++ ListRunLumi.cpp -o RunListRunLumi \
		`root-config --cflags --libs`

RunMatchFiles: MatchFiles.cpp
	g++ MatchFiles.cpp -o RunMatchFiles

RunPrepareMatching: PrepareMatching.cpp
	g++ PrepareMatching.cpp -o RunPrepareMatching
