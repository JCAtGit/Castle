; ModuleID = 'file4_origin_1_CLANGOPT.out.0.2.internalize.bc'
source_filename = "ld-temp.o"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@llvm.compiler.used = appending global [1 x ptr] [ptr @codemarker_alpha], section "llvm.metadata"
@.str = private unnamed_addr constant [27 x i8] c"compute_something(5) = %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [22 x i8] c"constant_fold() = %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [28 x i8] c"branch_elimination(0) = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"cse_example(3,4) = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [30 x i8] c"inline_candidate(10,20) = %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [19 x i8] c"max(100,200) = %d\0A\00", align 1
@.str.6 = private unnamed_addr constant [16 x i8] c"grade(85) = %c\0A\00", align 1
@.str.7 = private unnamed_addr constant [20 x i8] c"algebraic(42) = %d\0A\00", align 1
@.str.8 = private unnamed_addr constant [20 x i8] c"bit_tricks(7) = %d\0A\00", align 1
@__const.main.src = private unnamed_addr constant [32 x i8] c"Hello optimizable world!\00\00\00\00\00\00\00\00", align 16
@.str.9 = private unnamed_addr constant [12 x i8] c"Copied: %s\0A\00", align 1
@.str.10 = private unnamed_addr constant [50 x i8] c"square = %d, add_five = %d, multiply_by_two = %d\0A\00", align 1

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

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main() #2 {
  %1 = alloca [32 x i8], align 16
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !10
  tail call void @codemarker_alpha(i32 noundef 30) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !11
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !12
  tail call void @codemarker_alpha(i32 noundef 31) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !13
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !14
  tail call void @codemarker_alpha(i32 noundef 0) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !15
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !16
  tail call void @codemarker_alpha(i32 noundef 32) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !17
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !18
  tail call void @codemarker_alpha(i32 noundef 1) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !19
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !20
  tail call void @codemarker_alpha(i32 noundef 2) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !21
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef 27)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !22
  tail call void @codemarker_alpha(i32 noundef 33) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !23
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 3) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !25
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !26
  tail call void @codemarker_alpha(i32 noundef 4) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !27
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !28
  tail call void @codemarker_alpha(i32 noundef 5) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !29
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !30
  tail call void @codemarker_alpha(i32 noundef 6) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !31
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !32
  tail call void @codemarker_alpha(i32 noundef 7) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !33
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1, i32 noundef 20)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !34
  tail call void @codemarker_alpha(i32 noundef 34) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !35
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !36
  tail call void @codemarker_alpha(i32 noundef 8) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !37
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !38
  tail call void @codemarker_alpha(i32 noundef 9) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !39
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !40
  tail call void @codemarker_alpha(i32 noundef 10) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !41
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2, i32 noundef 48)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !42
  tail call void @codemarker_alpha(i32 noundef 35) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !43
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !44
  tail call void @codemarker_alpha(i32 noundef 11) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !45
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !46
  tail call void @codemarker_alpha(i32 noundef 12) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !47
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.3, i32 noundef 24)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !48
  tail call void @codemarker_alpha(i32 noundef 36) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !49
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.4, i32 noundef 120)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !50
  tail call void @codemarker_alpha(i32 noundef 37) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !51
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !52
  tail call void @codemarker_alpha(i32 noundef 19) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !53
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.5, i32 noundef 200)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !54
  tail call void @codemarker_alpha(i32 noundef 38) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !55
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !56
  tail call void @codemarker_alpha(i32 noundef 20) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !57
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.6, i32 noundef 66)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !58
  tail call void @codemarker_alpha(i32 noundef 39) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !59
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.7, i32 noundef 3)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !60
  tail call void @codemarker_alpha(i32 noundef 40) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !61
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.8, i32 noundef 42)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !62
  tail call void @codemarker_alpha(i32 noundef 41) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !63
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !64
  tail call void @codemarker_alpha(i32 noundef 42) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !65
  call void @llvm.lifetime.start.p0(i64 32, ptr nonnull %1) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !66
  tail call void @codemarker_alpha(i32 noundef 43) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !67
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !68
  tail call void @codemarker_alpha(i32 noundef 26) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !69
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 16 dereferenceable(32) %1, ptr noundef nonnull readonly align 16 dereferenceable(32) @__const.main.src, i64 32, i1 false)
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !70
  tail call void @codemarker_alpha(i32 noundef 44) #5
  tail call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !71
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.9, ptr noundef nonnull %1)
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !72
  call void @codemarker_alpha(i32 noundef 45) #5
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !73
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !74
  call void @codemarker_alpha(i32 noundef 46) #5
  call void asm sideeffect "", "~{memory},~{dirflag},~{fpsr},~{flags}"() #5, !srcloc !75
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.10, i32 noundef 100, i32 noundef 25, i32 noundef 20)
  call void @llvm.lifetime.end.p0(i64 32, ptr nonnull %1) #5
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #4

attributes #0 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #5 = { nounwind }

!llvm.ident = !{!0}
!llvm.module.flags = !{!1, !2, !3, !4}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 1, !"ThinLTO", i32 0}
!4 = !{i32 1, !"EnableSplitLTOUnit", i32 1}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = !{i64 27399}
!10 = !{i64 7227}
!11 = !{i64 7289}
!12 = !{i64 7349}
!13 = !{i64 7411}
!14 = !{i64 954}
!15 = !{i64 1015}
!16 = !{i64 7480}
!17 = !{i64 7542}
!18 = !{i64 1243}
!19 = !{i64 1304}
!20 = !{i64 1413}
!21 = !{i64 1474}
!22 = !{i64 7642}
!23 = !{i64 7704}
!24 = !{i64 1648}
!25 = !{i64 1709}
!26 = !{i64 1760}
!27 = !{i64 1821}
!28 = !{i64 1872}
!29 = !{i64 1933}
!30 = !{i64 1993}
!31 = !{i64 2054}
!32 = !{i64 2114}
!33 = !{i64 2175}
!34 = !{i64 7794}
!35 = !{i64 7856}
!36 = !{i64 2390}
!37 = !{i64 2451}
!38 = !{i64 2501}
!39 = !{i64 2562}
!40 = !{i64 2730}
!41 = !{i64 2792}
!42 = !{i64 7958}
!43 = !{i64 8020}
!44 = !{i64 2960}
!45 = !{i64 3022}
!46 = !{i64 3077}
!47 = !{i64 3139}
!48 = !{i64 8113}
!49 = !{i64 8175}
!50 = !{i64 8282}
!51 = !{i64 8344}
!52 = !{i64 4992}
!53 = !{i64 5054}
!54 = !{i64 8429}
!55 = !{i64 8491}
!56 = !{i64 5222}
!57 = !{i64 5284}
!58 = !{i64 8569}
!59 = !{i64 8631}
!60 = !{i64 8717}
!61 = !{i64 8779}
!62 = !{i64 8866}
!63 = !{i64 8928}
!64 = !{i64 9010}
!65 = !{i64 9072}
!66 = !{i64 9125}
!67 = !{i64 9187}
!68 = !{i64 6105}
!69 = !{i64 6167}
!70 = !{i64 9249}
!71 = !{i64 9311}
!72 = !{i64 9380}
!73 = !{i64 9442}
!74 = !{i64 9501}
!75 = !{i64 9563}
