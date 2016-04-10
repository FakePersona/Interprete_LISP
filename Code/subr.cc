
public:
  Not_Subr():
    runtime_error("Subroutine error") {  
  }
};

Object handle_subr(Object f,int* lvals){
	if (Object_to_string(f) == "+") {
		return do_plus(lvals);
	}
	if (Object_to_string(f) == "*") {
		return do_times(lvals);
	}
	throw Not_Subr();
}
