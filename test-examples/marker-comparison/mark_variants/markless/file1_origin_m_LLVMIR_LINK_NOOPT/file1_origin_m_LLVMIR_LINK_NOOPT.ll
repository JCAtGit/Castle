; ModuleID = './file1_origin_m_LLVMIR_LINK_NOOPT/./file1_origin_m_LLVMIR_LINK_NOOPT.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@.str = private unnamed_addr constant [51 x i8] c"Results: a=%d, b=%d, c=%d, d=%d, e=%d, f=%d, g=%d\0A\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @identity(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  ret i32 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 noundef %0, ptr noundef %1) #0 {
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca ptr, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i32, align 4
  store i32 0, ptr %3, align 4
  store i32 %0, ptr %4, align 4
  store ptr %1, ptr %5, align 8
  %13 = load i32, ptr %4, align 4
  %14 = add nsw i32 %13, 10
  store i32 %14, ptr %6, align 4
  %15 = load i32, ptr %4, align 4
  %16 = mul nsw i32 %15, 2
  store i32 %16, ptr %7, align 4
  store i32 42, ptr %8, align 4
  %17 = load i32, ptr %6, align 4
  %18 = call i32 @square(i32 noundef %17)
  store i32 %18, ptr %9, align 4
  %19 = load i32, ptr %7, align 4
  %20 = call i32 @add_five(i32 noundef %19)
  store i32 %20, ptr %10, align 4
  %21 = load i32, ptr %8, align 4
  %22 = call i32 @identity(i32 noundef %21)
  store i32 %22, ptr %11, align 4
  %23 = load i32, ptr %6, align 4
  %24 = mul nsw i32 %23, 2
  %25 = load i32, ptr %6, align 4
  %26 = mul nsw i32 %25, 2
  %27 = add nsw i32 %24, %26
  store i32 %27, ptr %12, align 4
  %28 = load i32, ptr %6, align 4
  %29 = load i32, ptr %7, align 4
  %30 = load i32, ptr %8, align 4
  %31 = load i32, ptr %9, align 4
  %32 = load i32, ptr %10, align 4
  %33 = load i32, ptr %11, align 4
  %34 = load i32, ptr %12, align 4
  %35 = call i32 (ptr, ...) @printf(ptr noundef @.str, i32 noundef %28, i32 noundef %29, i32 noundef %30, i32 noundef %31, i32 noundef %32, i32 noundef %33, i32 noundef %34)
  ret i32 0
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @square(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  %5 = mul nsw i32 %3, %4
  ret i32 %5
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i32 @add_five(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %3 = load i32, ptr %2, align 4
  %4 = add nsw i32 %3, 5
  ret i32 %4
}

declare dso_local i32 @printf(ptr noundef, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.ident = !{!0}
!llvm.module.flags = !{!1, !2, !3}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 2}
