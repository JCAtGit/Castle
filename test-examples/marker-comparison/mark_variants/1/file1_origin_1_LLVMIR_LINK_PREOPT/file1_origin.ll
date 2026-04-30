; ModuleID = './file1_origin_1_LLVMIR_LINK_PREOPT/file1_origin.bc'
source_filename = "file1_origin.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@.str = private unnamed_addr constant [51 x i8] c"Results: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d\0A\00", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @codemarker_alpha], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define internal void @codemarker_alpha(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %2)
  store volatile i32 %0, ptr %2, align 4, !tbaa !5
  tail call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %0) #5, !srcloc !9
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %2)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @identity(i32 noundef returned %0) local_unnamed_addr #2 {
  ret i32 %0
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr noundef readnone captures(none) %1) local_unnamed_addr #3 {
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !10
  tail call void @codemarker_alpha(i32 noundef 0) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !11
  %3 = add nsw i32 %0, 10
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !12
  tail call void @codemarker_alpha(i32 noundef 1) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !13
  %4 = shl nsw i32 %0, 1
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !14
  tail call void @codemarker_alpha(i32 noundef 2) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !15
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !16
  tail call void @codemarker_alpha(i32 noundef 3) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !17
  %5 = mul nsw i32 %3, %3
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !18
  tail call void @codemarker_alpha(i32 noundef 4) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !19
  %6 = add nsw i32 %4, 5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !20
  tail call void @codemarker_alpha(i32 noundef 5) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !21
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !22
  tail call void @codemarker_alpha(i32 noundef 6) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !23
  %7 = shl nsw i32 %3, 2
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 7) #5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !25
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef %3, i32 noundef %4, i32 noundef 42, i32 noundef %5, i32 noundef %6, i32 noundef 42, i32 noundef %7)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

attributes #0 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { mustprogress nofree noinline norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 1, !"ThinLTO", i32 0}
!3 = !{i32 1, !"EnableSplitLTOUnit", i32 1}
!4 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{i64 20208}
!10 = !{i64 918}
!11 = !{i64 985}
!12 = !{i64 1066}
!13 = !{i64 1133}
!14 = !{i64 1246}
!15 = !{i64 1313}
!16 = !{i64 1418}
!17 = !{i64 1485}
!18 = !{i64 1588}
!19 = !{i64 1655}
!20 = !{i64 1852}
!21 = !{i64 1919}
!22 = !{i64 2097}
!23 = !{i64 2164}
!24 = !{i64 2321}
!25 = !{i64 2388}

^0 = module: (path: "./file1_origin_1_LLVMIR_LINK_PREOPT/file1_origin.bc", hash: (0, 0, 0, 0, 0))
^1 = gv: (name: "llvm.lifetime.start.p0") ; guid = 4736283418132453162
^2 = gv: (name: "codemarker_alpha", summaries: (function: (module: ^0, flags: (linkage: internal, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 6, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 1, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0)))) ; guid = 5349270672727963487
^3 = gv: (name: "printf") ; guid = 7383291119112528047
^4 = gv: (name: ".str", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 9257082116499937833
^5 = gv: (name: "llvm.compiler.used", summaries: (variable: (module: ^0, flags: (linkage: appending, visibility: default, notEligibleToImport: 1, live: 1, dsoLocal: 0, canAutoHide: 0, importType: definition), varFlags: (readonly: 0, writeonly: 0, constant: 0), refs: (^2)))) ; guid = 9610627770985738006
^6 = gv: (name: "identity", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 1, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 1, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 9982670531020540159
^7 = gv: (name: "llvm.lifetime.end.p0") ; guid = 13244677477227836793
^8 = gv: (name: "main", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 31, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 1, mustBeUnreachable: 0), calls: ((callee: ^2, tail: 1), (callee: ^3, tail: 1)), refs: (^4)))) ; guid = 15822663052811949562
^9 = flags: 8
^10 = blockcount: 0
