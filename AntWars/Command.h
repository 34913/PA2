#pragma once

class Command
{
public:

	// basic commands

	static Command nextBase;
	static Command backBase;
	static Command trainMelee;
	static Command trainRange;
	static Command trainTank;
	static Command pause;

	const int code;

	Command(int code);

	// operators
	bool operator == (const Command& com) const;

	bool operator != (const Command& com) const;

};
