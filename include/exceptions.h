#pragma once

namespace tcexc{
	class NotImplementedException : public std::logic_error{
	public:
		NotImplementedException () : std::logic_error{"Function not implemented for this class."} {}
	};
}