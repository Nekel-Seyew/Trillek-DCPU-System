#include <cstdint>
#include <ctype.h>
#define word_2 uint32_t
#define word uint16_t
#define half uint8_t

class Basic_cpu{
	private:
		word mem[10000];
		word reg[8];
		word PC, SP, EX, IA;
		
	public:
		Basic_cpu();
		word* opr(word code);
		void skip();
		void step();


};
