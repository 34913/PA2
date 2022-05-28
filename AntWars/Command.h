#pragma once

class Command
{
public:

	const int code;

	static Command nextBase;
	static Command backBase;
	static Command trainMelee;
	static Command trainRange;
	static Command trainTank;
	static Command pause;

	Command(int code);

	bool operator == (const Command& com) const;

	bool operator != (const Command& com) const;

};
