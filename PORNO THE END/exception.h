#pragma once
#include <exception>
#include <string>
enum except_type {
	MemoryAllocateError, IndexOutOfRange, SizeBelowZero, NegativeRange,
	IncorrectComparator, IncorrectRange, IncorrectValue, IncorrectInputFormat, IncorrectIterator,
	NoRequiredArgument, NoSuchEdge, CannotWriteFile, CannotReadFile, EmptyAP, IncorrectPath, GraphNotConnective, EmptySM, UnknownFlag, NodeExists, UnknownCommand, NoSuchElement, IncorrectArraySize, MutuallyExclusive, TooManySort, EmptySequence
};
#include<iostream>

class SetException {
public:
	except_type id;
	SetException(except_type id) { this->id = id; }
	const char* message() {
		if (id == MemoryAllocateError) return "\n[Error] Cannot allocate memory\n";
		if (id == IndexOutOfRange) return "\n[Error] Subscirpt index out of range\n";
		if (id == SizeBelowZero) return "\n[Error] Size below zero\n";
		if (id == NegativeRange) return "\n[Error] Range is less than zero\n";
		if (id == CannotWriteFile) return "\n[Error] Cannot to write file\n";
		if (id == CannotReadFile) return "\n[Error] Cannot to read file\n";
		if (id == IncorrectPath) return "\n[Error] Some edges in path aren't com[atible\n";
		if (id == IncorrectArraySize) return "\n[Error] Range of array isn't correct\n";
		if (id == IncorrectComparator) return "\n[Error] Incorrect sorting comparator\n";
		if (id == GraphNotConnective) return "\n[Error] Two nodes aren't connective";
		if (id == IncorrectRange) return "\n[Error] The size of sequence isn't equal to vertical size\n";
		if (id == IncorrectValue) return "\n[Error] Not allowed parameter value\n";
		if (id == NoRequiredArgument) return "\n[Error] No required argument for command\n";
		if (id == UnknownFlag) return "\n[Error] Unknown flag name for this command\n";
		if (id == UnknownCommand) return "\n[Error] Unknown command name\n";
		if (id == NodeExists) return "\n[Error] This node is already exists\n";
		if (id == IncorrectInputFormat) return "\n[Error] Incorrect Input format\n";
		if (id == MutuallyExclusive) return "\n[Error] There are two or more mutually exclusive arguments\n";
		if (id == TooManySort) return "\n[Error] You cannot compare too much function at the same time\n";
		if (id == NoSuchEdge) return "\n[Error] There is no such edge exists\n";
		if (id == EmptySequence) return "\n[Error] Your histogram hasn't created yet\n";
		if (id == EmptyAP) return "\n[Error] Your alphabet pointer hasn't created yet\n";
		if (id == EmptySM) return "\n[Error] Your sparse matrix hasn't created yet\n";
		if (id == NoSuchElement) return "\n[Error] There are no such node(-s) or edge(-s) had found\n";
	}
};