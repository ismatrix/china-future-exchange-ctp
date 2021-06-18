
module Test
{
	struct T
	{
		int age;
		string name;
	};


	struct TTT
	{
		T t;
		string msg;

	};

	sequence<T> TType;

	dictionary<string, T> MapType;

	interface TT
	{
		void push(TType t, MapType tt);
	};

};
