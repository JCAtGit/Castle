; ModuleID = './file5_origin_2_LLVMIR_LINK_NOOPT/file5_origin.bc'
source_filename = "file5_origin.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@llvm.compiler.used = appending global [1 x ptr] [ptr @codemarker_alpha], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @codemarker_alpha(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %3) #1, !srcloc !4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fnC() #0 {
  %1 = alloca i32, align 4
  call void @codemarker_alpha(i32 noundef 0) #1
  store i32 0, ptr %1, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fnB() #0 {
  %1 = alloca i32, align 4
  call void @codemarker_alpha(i32 noundef 1) #1
  store i32 0, ptr %1, align 4
  call void @codemarker_alpha(i32 noundef 2) #1
  call void @fnC()
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @fnA() #0 {
  %1 = alloca i32, align 4
  call void @codemarker_alpha(i32 noundef 3) #1
  store i32 0, ptr %1, align 4
  call void @codemarker_alpha(i32 noundef 4) #1
  call void @fnB()
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca ptr, align 8
  store i32 %0, ptr %3, align 4
  store ptr %1, ptr %4, align 8
  call void @codemarker_alpha(i32 noundef 5) #1
  call void @fnA()
  ret i32 0
}

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!4 = !{i64 17957}
