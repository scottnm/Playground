import cpptestlib
import CxxStdlib

cpptestlib.CppTestLib.TakeSomeCppParams("test123");

var i: Int32 = 5;
cpptestlib.CppTestLib.TakeAPtrParam(&i);

let cval: Character = "Z";
let inVal = cpptestlib.CppTestLib.SomeStruct(
    ival: 30, 
    cval: CChar(cval.asciiValue!));
withUnsafePointer(to: inVal) { inValPtr in
    cpptestlib.CppTestLib.TakeAStructPtrParam(inValPtr);
}
// 
// CppTestLib::SomeStruct outVal;
// CppTestLib::OutStructPtrParam(&outVal);
// 
// const int rv = CppTestLib::ReturnSomePodValue(10);
// NSLog(@".. received %i from CppTestLib::ReturnSomePodValue", rv);
print("Hello, World!"); 