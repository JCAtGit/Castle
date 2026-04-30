; ModuleID = './file5_origin_0_LLVMIR_LINK_NOOPT/./file5_origin_0_LLVMIR_LINK_NOOPT.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fnC() #0 {
  %1 = alloca i32, align 4
  call void @codemarker_alpha(i32 noundef 0)
  store i32 0, ptr %1, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fnB() #0 {
  %1 = alloca i32, align 4
  call void @codemarker_alpha(i32 noundef 1)
  store i32 0, ptr %1, align 4
  call void @codemarker_alpha(i32 noundef 2)
  call void @fnC()
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fnA() #0 {
  %1 = alloca i32, align 4
  call void @codemarker_alpha(i32 noundef 3)
  store i32 0, ptr %1, align 4
  call void @codemarker_alpha(i32 noundef 4)
  call void @fnB()
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %0, ptr %3, align 4
  store ptr %1, ptr %4, align 8
  call void @codemarker_alpha(i32 noundef 5)
  call void @fnA()
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @codemarker_alpha(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  store volatile i32 %4, ptr %3, align 4
  ret void
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2, !3}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
