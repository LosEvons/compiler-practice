#pragma once
#include <vector>
#include <string>

class Diagnostics{
public:
	static std::vector<std::string> GetDiagnostics() { return _diagnostics; };
	//static void AddDiagnostics(std::string str) { _diagnostics.push_back(str); };
	static std::vector<std::string> _diagnostics;
private:
	
};