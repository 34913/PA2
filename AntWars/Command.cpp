#include "Command.h"

Command Command::nextBase(0);
Command Command::backBase(1);
Command Command::trainMelee(2);
Command Command::trainRange(3);
Command Command::trainTank(4);
Command Command::pause(5);

Command::Command(int code)
	:code(code)
{}

bool Command::operator==(const Command& com) const
{
	return code == com.code;
}

bool Command::operator!=(const Command& com) const
{
	return !(*this == com);
}
