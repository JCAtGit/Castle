; ModuleID = './file5_origin_2_LLVMIR_LINK_PREOPT/file5_origin.bc'
source_filename = "file5_origin.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@llvm.compiler.used = appending global [1 x ptr] [ptr @codemarker_alpha], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define dso_local void @codemarker_alpha(i32 noundef %0) #0 {
  tail call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %0) #2, !srcloc !5
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @fnC() local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 0) #2
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @fnB() local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 1) #2
  tail call void @codemarker_alpha(i32 noundef 2) #2
  tail call void @codemarker_alpha(i32 noundef 0) #2
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @fnA() local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 3) #2
  tail call void @codemarker_alpha(i32 noundef 4) #2
  tail call void @codemarker_alpha(i32 noundef 1) #2
  tail call void @codemarker_alpha(i32 noundef 2) #2
  tail call void @codemarker_alpha(i32 noundef 0) #2
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr noundef readnone captures(none) %1) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 5) #2
  tail call void @codemarker_alpha(i32 noundef 3) #2
  tail call void @codemarker_alpha(i32 noundef 4) #2
  tail call void @codemarker_alpha(i32 noundef 1) #2
  tail call void @codemarker_alpha(i32 noundef 2) #2
  tail call void @codemarker_alpha(i32 noundef 0) #2
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 1, !"ThinLTO", i32 0}
!3 = !{i32 1, !"EnableSplitLTOUnit", i32 1}
!4 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!5 = !{i64 17956}

^0 = module: (path: "./file5_origin_2_LLVMIR_LINK_PREOPT/file5_origin.bc", hash: (0, 0, 0, 0, 0))
^1 = gv: (name: "fnC", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^3, tail: 1))))) ; guid = 2821593531812661402
^2 = gv: (name: "llvm.compiler.used", summaries: (variable: (module: ^0, flags: (linkage: appending, visibility: default, notEligibleToImport: 1, live: 1, dsoLocal: 0, canAutoHide: 0, importType: definition), varFlags: (readonly: 0, writeonly: 0, constant: 0), refs: (^3)))) ; guid = 9610627770985738006
^3 = gv: (name: "codemarker_alpha", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 1, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0)))) ; guid = 12546640360034877224
^4 = gv: (name: "fnA", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 6, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^3, tail: 1))))) ; guid = 15009740524810852068
^5 = gv: (name: "main", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 7, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^3, tail: 1))))) ; guid = 15822663052811949562
^6 = gv: (name: "fnB", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 4, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^3, tail: 1))))) ; guid = 18369480006499705232
^7 = flags: 8
^8 = blockcount: 0
