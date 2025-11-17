import cpptestlib
import CxxStdlib

print("Hello, World!"); 

let test123str = std.string("test123");
cpptestlib.CppTestLib.TakeSomeCppParams(test123str);

var i: Int32 = 5;
cpptestlib.CppTestLib.TakeAPtrParam(&i);

let cval: Character = "Z";
let inVal = cpptestlib.CppTestLib.SomeStruct(
    ival: 30, 
    cval: CChar(cval.asciiValue!));
withUnsafePointer(to: inVal) { inValPtr in
    cpptestlib.CppTestLib.TakeAStructPtrParam(inValPtr);
}

// FIXME: is there a way to *NOT* initialize outVal?
var outVal  = cpptestlib.CppTestLib.SomeStruct(ival: 0, cval: 0);
cpptestlib.CppTestLib.OutStructPtrParam(&outVal);
let outValCvalChar = Character(UnicodeScalar(UInt8(outVal.cval)))
print("OutStructPtr Param result = \(outVal.ival) \(outValCvalChar)")

let rv = cpptestlib.CppTestLib.ReturnSomePodValue(10);
print(".. received \(rv) from CppTestLib::ReturnSomePodValue");