#include "File.h"

void run() {

}

std::array<int, 99> touch =
{
	8,
9  ,
13 ,
16 ,
17 ,
18 ,
19 ,
20 ,
27 ,
33 ,
34 ,
35 ,
36 ,
37 ,
37 ,
38 ,
39 ,
40 ,
45 ,
46 ,
48 ,
49 ,
50 ,
51 ,
52 ,
53 ,
54 ,
55 ,
56 ,
57 ,
65 ,
66 ,
67 ,
68 ,
69 ,
70 ,
71 ,
72 ,
73 ,
74 ,
75 ,
76 ,
77 ,
78 ,
79 ,
80 ,
81 ,
82 ,
83 ,
84 ,
85 ,
86 ,
87 ,
88 ,
89 ,
90 ,
91 ,
92 ,
93 ,
96 ,
97 ,
98 ,
99 ,
100,
101,
102,
103,
104,
105,
106,
107,
109,
110,
111,
112,
113,
114,
115,
116,
117,
118,
119,
120,
121,
122,
123,
144,
145,
186,
187,
188,
189,
190,
191,
192,
219,
220,
221,
222
};

void testarr() {
	for (int i = 0; i < touch.size(); ++i) {
		std::cout << touch.at(i)<<' ' << Logger.codes.at(touch.at(i)) << std::endl;
	}
}

int main()
{
	Logger.record();
	//File.saveMacroCalls("C:/dev/demo.txt");
	//run();

	//testarr();
	//Logger.printFrames();
	return 0;
}