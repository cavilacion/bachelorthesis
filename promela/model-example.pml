/* Found 4 candidate stutter states
 * and removed 3 actual spurious self-loops.
 */
bool a = 0;
bool b = 0;
bool d = 0;
bool e = 0;

active proctype FSA() {
q_init:
	if
	:: (true) -> goto q_0;
	:: (true) -> goto q_1;
	:: (true) -> goto q_2;
	:: (true) -> goto q_3;
	:: (true) -> goto q_4;
	:: (true) -> goto q_5;
	:: (true) -> goto q_6;
	:: (true) -> goto q_7;
	:: (true) -> goto q_8;
	:: (true) -> goto q_9;
	:: (true) -> goto q_10;
	:: (true) -> goto q_11;
	fi
q_0:		/* I_q = [ (0,0) , (1,1) ] */
	atomic { a=0; b=1; d=0; e=1;  } 
	if
	:: true -> goto q_0;
	fi
q_1:		/* I_q = [ (1,0) , (3,1) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_0;
	fi
q_2:		/* I_q = [ (3,0) , (4,1) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_1;
	fi
q_3:		/* I_q = [ (4,0) , (6,1) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_1;
	:: true -> goto q_2;
	:: true -> goto q_5;
	:: true -> goto q_6;
	fi
q_4:		/* I_q = [ (0,1) , (1,3) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_0;
	fi
q_5:		/* I_q = [ (1,1) , (3,3) ] */
	atomic { a=0; b=0; d=1; e=1;  } 
	if
	:: true -> goto q_0;
	:: true -> goto q_1;
	:: true -> goto q_4;
	fi
q_6:		/* I_q = [ (3,1) , (4,3) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_1;
	:: true -> goto q_5;
	fi
q_7:		/* I_q = [ (4,1) , (6,3) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_1;
	:: true -> goto q_2;
	:: true -> goto q_5;
	:: true -> goto q_6;
	fi
q_8:		/* I_q = [ (0,3) , (1,4) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_4;
	fi
q_9:		/* I_q = [ (1,3) , (3,4) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_4;
	:: true -> goto q_5;
	fi
q_10:		/* I_q = [ (3,3) , (4,4) ] */
	atomic { a=1; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_5;
	fi
q_11:		/* I_q = [ (4,3) , (6,4) ] */
	atomic { a=0; b=0; d=0; e=1;  } 
	if
	:: true -> goto q_5;
	:: true -> goto q_6;
	fi
}

never  {    /* ! <> [] b */
T0_init:
	do
	:: (! ((b))) -> goto accept_S9
	:: (1) -> goto T0_init
	od;
accept_S9:
	do
	:: (1) -> goto T0_init
	od;
}
