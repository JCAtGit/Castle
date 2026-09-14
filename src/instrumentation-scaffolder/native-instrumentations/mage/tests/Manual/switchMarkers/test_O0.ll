; ModuleID = 'test.c'
source_filename = "test.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@.str = private unnamed_addr constant [5 x i8] c"One\0A\00", align 1
@.str.1 = private unnamed_addr constant [5 x i8] c"Two\0A\00", align 1
@llvm.compiler.used = appending global [3 x ptr] [ptr @mage_alpha83, ptr @mage_alpha84, ptr @mage_alpha85], section "llvm.metadata"

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @mage_alpha83(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  store volatile i32 %4, ptr %3, align 4
  %5 = load i32, ptr %2, align 4
  call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %5) #3, !srcloc !4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @mage_alpha84(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  store volatile i32 %4, ptr %3, align 4
  %5 = load i32, ptr %2, align 4
  call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %5) #3, !srcloc !5
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local void @mage_alpha85(i32 noundef %0) #0 {
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 %0, ptr %2, align 4
  %4 = load i32, ptr %2, align 4
  store volatile i32 %4, ptr %3, align 4
  %5 = load i32, ptr %2, align 4
  call void asm sideeffect "", "r,~{memory},~{dirflag},~{fpsr},~{flags}"(i32 %5) #3, !srcloc !6
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %3 = call i64 @time(ptr noundef null) #3
  %4 = trunc i64 %3 to i32
  call void @srand(i32 noundef %4) #3
  %5 = call i32 @rand() #3
  store i32 %5, ptr %2, align 4
  %6 = load i32, ptr %2, align 4
  switch i32 %6, label %12 [
    i32 1, label %7
    i32 2, label %10
  ]

7:                                                ; preds = %0
  %8 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %9

9:                                                ; preds = %7
  br label %10llvm 

10:                                               ; preds = %0, %9
  %11 = call i32 (ptr, ...) @printf(ptr noundef @.str.1)
  call void @mage_alpha85(i32 noundef 85)
  br label %12

12:                                               ; preds = %10, %0
  %13 = load i32, ptr %1, align 4
  ret i32 %13
}

; Function Attrs: nounwind
declare dso_local void @srand(i32 noundef) #1

; Function Attrs: nounwind
declare dso_local i64 @time(ptr noundef) #1

; Function Attrs: nounwind
declare dso_local i32 @rand() #1

declare dso_local i32 @printf(ptr noundef, ...) #2

attributes #0 = { noinline nounwind optnone uwtable "frame-pointer"="all" "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { "frame-pointer"="all" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0, !1, !2}
!llvm.ident = !{!3}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 7, !"frame-pointer", i32 2}
!3 = !{!"clang version 21.1.8 (Fedora 21.1.8-6.fc43)"}
!4 = !{i64 239}
!5 = !{i64 403}
!6 = !{i64 567}
