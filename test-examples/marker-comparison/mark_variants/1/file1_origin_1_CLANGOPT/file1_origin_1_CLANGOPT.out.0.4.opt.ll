; ModuleID = 'file1_origin_1_CLANGOPT.out.0.4.opt.bc'
source_filename = "ld-temp.o"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@.str = private unnamed_addr constant [51 x i8] c"Results: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d\0A\00", align 1
@llvm.compiler.used = appending global [1 x ptr] [ptr @codemarker_alpha], section "llvm.metadata"

; Function Attrs: noinline nounwind uwtable
define internal void @codemarker_alpha(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %2)
  store volatile i32 %0, ptr %2, align 4, !tbaa !5
  tail call void asm sideeffect "", "r,~{rax},~{rcx},~{rdx},~{rsi},~{rdi},~{r8},~{r9},~{r10},~{r11},~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %0) #4, !srcloc !9
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %2)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr noundef readnone captures(none) %1) local_unnamed_addr #2 {
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !10
  tail call void @codemarker_alpha(i32 noundef 0) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !11
  %3 = add nsw i32 %0, 10
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !12
  tail call void @codemarker_alpha(i32 noundef 1) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !13
  %4 = shl nsw i32 %0, 1
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !14
  tail call void @codemarker_alpha(i32 noundef 2) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !15
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !16
  tail call void @codemarker_alpha(i32 noundef 3) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !17
  %5 = mul nsw i32 %3, %3
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !18
  tail call void @codemarker_alpha(i32 noundef 4) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !19
  %6 = add nsw i32 %4, 5
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !20
  tail call void @codemarker_alpha(i32 noundef 5) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !21
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !22
  tail call void @codemarker_alpha(i32 noundef 6) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !23
  %7 = shl nsw i32 %3, 2
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !24
  tail call void @codemarker_alpha(i32 noundef 7) #4
  tail call void asm sideeffect "", "~{memory},~{cc},~{dirflag},~{fpsr},~{flags}"() #4, !srcloc !25
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef %3, i32 noundef %4, i32 noundef 42, i32 noundef %5, i32 noundef %6, i32 noundef 42, i32 noundef %7)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

attributes #0 = { noinline nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nounwind }

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
