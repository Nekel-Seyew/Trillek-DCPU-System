#include <cstdlib>
#include "basic_DCPU.hpp"

Basic_cpu::Basic_cpu()
{
	
}

static half skiptable[32] = { /* operand forms that advance pc */
		[0x10] = 1, [0x11] = 1, [0x12] = 1, [0x13] = 1,
		[0x14] = 1, [0x15] = 1, [0x16] = 1, [0x17] = 1,
		[0x1E] = 1, [0x1F] = 1,
		};
word* Basic_cpu::opr(word code)
{
	if(code < 0x08){ //returns the proper register value
		return this->reg[code];
	}
	/* Returns the value of memory located at the 
	 * location stored in the register.
	 * The (code & 7) part gets the register value
	 */
	if((code - 0x08) < 0x08){
		return this->mem + this->reg[code & 7];
	}
	/* Returns the value in memory located at the location stored in the
	 * register provided by the word, plus the value in memory stored at
	 * the location of the program counter (which is then incremented).
	 */
	if((code - 0x10) < 0x08){
		return this->m +
			((this->reg[code & 7] +this->mem[this->PC++]) &&0xffff);
	}
	switch(code){
	case 0x18:
		return this->mem + this->SP++;
	case 0x19:
		return this->mem + this->SP;
	case 0x1a:
		return this->mem + (--(this->SP));
	case 0x1b:
		return &(this->SP);
	case 0x1c:
		return &(this->PC);
	case 0x1d:
		return &(this->EX);
	case 0x1e:
		return d->mem + this->mem[this->PC++];
	case 0x1f:
		return this->mem + this->PC++;
	default:
		return (code & 0x1f);
	}
}

void Basic_cpu::skip()
{
	word op=this->mem[this->PC++];
	this->PC+=skiptable[op>>10];
	if((op&15) == 0){
		this->PC+=skiptable[(op >> 4) & 31];
	}
}
//MODIFIED IMPLEMENTATION. NOT PROPER. NEED TO FIX
//http://dcpu.com/dcpu-16/
void Basic_cpu::step()
{
	word op= this->mem[this->PC++];
	word dest;
	word_2 res;
	word a, b, *aa;

	aa=this->opr(dst = (op >> 4) & 0x3f);
	a = *aa;
	b = *this->opr(op >> 10);

	switch(op & 0xf){
	case 0x1:{
		     res = b;
		     break;
		 }
	case 0x2:{
			res = a+b;
			this->EX=res>>16;
			break;
		 }
	case 0x3:{
			res = a-b;
			this->EX=res>>16;
			break;
		 }
	case 0x4:{
			res = a*b;
			this->EX=res>>16;
			break;
		 }
	case 0x5:{
			if(b){
				res = a/b;
			}else{
				res=0;
			}
			this->EX=res>>16;
			break;
		 }
	case 0x6:{
			if(b){
				res = a%b;
			}else{
				res=0;
			}
			break;
		 }
	case 0x7:{
			res= a<<b;
			this->EX = res>>16;
			break;
		 }
	case 0x8:{
			res= a>>b;
			this->EX=res>>16;
			break;
		 }
	case 0x9:{
			res= a&b;
			break;
		 }
	case 0xA:{
			res= a | b;
			break;
		 }
	case 0xB:{
			res= a^b;
			break;
		 }
	case 0xC:{
			if(a!=b){
				this->skip();
			}
			break;
		 }
	case 0xD:{
			if(a==b){
				this->skip();
			}
			break;
		 }
	case 0xE:{
			if(a<=b){
				this->skip();
			}
			break;
		 }
	case 0xF:{
			if((a&b)==0){
				this->skip();
			}
			break;
		 }
	}
	if(dst < 0x1f){
		*aa=res;
	}
	return;
}
