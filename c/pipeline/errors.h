#ifndef __ERRORS_H__
#define __ERRORS_H__

#ifdef __cpluscplus
export "C" {
#endif // __cpluscplus


// function return value:
// error: >= 0 - success£¬ - 1 error

enum {
	success,
};

const char* errors[] = {
	"success",
	"error one",
	"error two",
	"error three",
	"error four",
	"error five",
	"error six",
	"error seven",
	"error eight",
	"error nine",
	"error ten"
};


#ifdef __cpluscplus
}
#endif // __cpluscplus

#endif // !__ERRORS_H__
