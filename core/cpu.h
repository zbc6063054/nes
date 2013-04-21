struct struct_cpu {
	u8 reg_a;						//register a
	u8 reg_x;						//register b
	u8 reg_y;						//register c

	u16 reg_pc;						//register pc
	u8 reg_flag;					//flag
}

enum register_type{
	reg_type_a,
	reg_type_x,
	reg_type_y
}

BYTE addr_imm();
BYTE addr_abs();
BYTE addr_zero();
BYTE addr_zero();
BYTE addr_reg(u8 reg);
BYTE addr_zero_reg(u8 reg);
BYTE addr_abs_reg(u8 reg);
BYTE addr_ind_x();
BYTE addr_ind_y();

extern struct struct_cpu cpu;
