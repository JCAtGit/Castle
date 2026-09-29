; ModuleID = 'testfile_attrib_optnone.c'
source_filename = "testfile_attrib_optnone.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@.str = private unnamed_addr constant [12 x i8] c"Hello World\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @testExpr() local_unnamed_addr #0 {
  ret i32 11
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %1 = alloca i32, align 4
  store i32 0, ptr %1, align 4
  %2 = call i32 @testExpr()
  br label %3

3:                                                ; preds = %0
  %4 = call i32 (ptr, ...) @printf(ptr noundef @.str)
  br label %5

5:                                                ; preds = %3
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

attributes #0 = { noinline nounwind optnone uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{!"clang version 21.1.8 (Fedora 21.1.8-6.fc43)"}
