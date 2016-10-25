#include "../h/PineconeParser.h"

ActionTablePtr table;

#define Dub_CPP double
#define Int_CPP int
#define Bool_CPP bool
#define Void_CPP char

#define Dub_PNCN Dub
#define Int_PNCN Int
#define Bool_PNCN Bool
#define Void_PNCN Void

#define CONCAT(a,b) a##_##b

#define getPncnType(typeIn) CONCAT(typeIn, PNCN)
#define getCppType(typeIn) CONCAT(typeIn, CPP)

#define LAMBDA_HEADER [](void* leftIn, void* rightIn)->void*

#define retrn out=

#define GET_PTR_VAL(typeIn, varInName) =*((getCppType(typeIn)*)varInName)

#define DO_INSTANTIATE(typeIn, varOutName, setVal) getCppType(typeIn) varOutName setVal;
#define DONT_INSTANTIATE(typeIn, varOutName, setVal) ;

#define DO_RETURN_VAL(typeIn, varName) void* outPtr=malloc(getPncnType(typeIn)->getSize()); memcpy(outPtr, &varName, getPncnType(typeIn)->getSize()); return outPtr;
#define DONT_RETURN_VAL(typeIn, varName) return nullptr;

#define INSTANTIATE_Dub DO_INSTANTIATE
#define INSTANTIATE_Int DO_INSTANTIATE
#define INSTANTIATE_Bool DO_INSTANTIATE
#define INSTANTIATE_Void DONT_INSTANTIATE

#define RETURN_Dub DO_RETURN_VAL
#define RETURN_Int DO_RETURN_VAL
#define RETURN_Bool DO_RETURN_VAL
#define RETURN_Void DONT_RETURN_VAL

#define func(nameText, returnType, leftType, rightType, lambdaBody)		\
addAction(nameText, getPncnType(returnType), getPncnType(leftType), getPncnType(rightType), LAMBDA_HEADER\
{																		\
	CONCAT(INSTANTIATE, leftType)(leftType, left, GET_PTR_VAL(leftType, leftIn))				\
	CONCAT(INSTANTIATE, rightType)(rightType, right, GET_PTR_VAL(rightType, rightIn))			\
	CONCAT(INSTANTIATE, returnType)(returnType, out, )			\
	lambdaBody;															\
	CONCAT(RETURN, returnType)(returnType, out)					\
})																		\

void addAction(Action* in)
{
	table->addAction(ActionPtr(in));
}

void addAction(Action* in, OperatorType opType)
{
	table->addAction(ActionPtr(in), opType);
}

void addAction(string text, Type returnType, Type leftType, Type rightType, function<void*(void*, void*)> lambda)
{
	addAction(new LambdaAction(returnType, lambda, leftType, rightType, text));
}

void addAction(OperatorType opType, Type returnType, Type leftType, Type rightType, function<void*(void*, void*)> lambda)
{
	addAction(new LambdaAction(returnType, lambda, leftType, rightType, OperatorElement::toString(opType)), opType);
}

void populatePineconeStdLib(ActionTablePtr t)
{
	table=t;
	
	//this makes a new void action after type constants have been created, if left to the original the Void type may not be set up yet
	voidAction=ActionPtr(new VoidAction());
	
	///operators
	
	func(OP_PLUS, Int, Int, Int,
		retrn left+right);
	
	func(OP_PLUS, Dub, Dub, Dub,
		retrn left+right);
	
	
	
	func(OP_MINUS, Dub, Dub, Dub,
		retrn left-right);
	
	func(OP_MINUS, Int, Int, Int,
		retrn left-right);
	
	
	///basic types
	
	table->addType(Void);
	table->addType(Bool);
	table->addType(Int);
	table->addType(Dub);
	
	
	///initalizers
	
	func("Bool", Bool, Void, Void, 
		retrn false);
	
	func("Int", Int, Void, Void, 
		retrn 0);
	
	func("Dub", Dub, Void, Void, 
		retrn 0.0);
		
	
	///casting
	
	func("Dub", Dub, Void, Int,
		retrn right);
	
	func("Int", Int, Void, Dub, 
		retrn right);
	
	
	///mscl functions
	
	//print
	
	func("print", Void, Void, Void,
		cout << endl);
	
	func("print", Void, Void, Bool,
		cout << (right?"tru":"fls") << endl);
	
	func("print", Void, Void, Int,
		cout << right << endl);
	
	func("print", Void, Void, Dub,
		cout << right << endl);
	
	
	/*t->addAction
	(
		ActionPtr
		(
			new LambdaAction
			(
				Type(Type::VOID),
				
				[](void* left, void* right)->void*
				{
					cout << *(int*)right << endl;
					return nullptr;
				},
				
				Type(Type::VOID), Type(Type::INT),
				
				"print"
			)
		)
	);*/
	
	//t->addAction()
	
	cout << endl << putStringInBox(table->toString(), false, "standard library") << endl;
	
	table=nullptr;
}

