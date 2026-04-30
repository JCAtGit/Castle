; ModuleID = './file5_origin_0_LLVMIR_LINK_PREOPT/./file5_origin_0_LLVMIR_LINK_PREOPT.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

; Function Attrs: nounwind uwtable
define dso_local void @fnC() local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 0) #3
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @fnB() local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 1) #3
  tail call void @codemarker_alpha(i32 noundef 2) #3
  tail call void @codemarker_alpha(i32 noundef 0) #3
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local void @fnA() local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 3) #3
  tail call void @codemarker_alpha(i32 noundef 4) #3
  tail call void @codemarker_alpha(i32 noundef 1) #3
  tail call void @codemarker_alpha(i32 noundef 2) #3
  tail call void @codemarker_alpha(i32 noundef 0) #3
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main(i32 noundef %0, ptr noundef readnone captures(none) %1) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 5) #3
  tail call void @codemarker_alpha(i32 noundef 3) #3
  tail call void @codemarker_alpha(i32 noundef 4) #3
  tail call void @codemarker_alpha(i32 noundef 1) #3
  tail call void @codemarker_alpha(i32 noundef 2) #3
  tail call void @codemarker_alpha(i32 noundef 0) #3
  ret i32 0
}

; Function Attrs: nofree norecurse nounwind memory(inaccessiblemem: readwrite) uwtable
define dso_local void @codemarker_alpha(i32 noundef %0) local_unnamed_addr #1 {
  %2 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %2)
  store volatile i32 %0, ptr %2, align 4, !tbaa !3
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %2)
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #2

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree norecurse nounwind memory(inaccessiblemem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nounwind }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
