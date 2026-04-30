; ModuleID = './file2_origin_0_LLVMIR_LINK_PREOPT/./file2_origin_0_LLVMIR_LINK_PREOPT.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }

@totalOperations = dso_local local_unnamed_addr global i32 0, align 4
@lastError = dso_local local_unnamed_addr global i32 0, align 4
@logCount = dso_local local_unnamed_addr global i32 0, align 4
@logMessages = dso_local global [100 x [256 x i8]] zeroinitializer, align 16
@.str = private unnamed_addr constant [54 x i8] c"ERROR: Invalid deductResource call (res=%d, amt=%.2f)\00", align 1
@resources = dso_local local_unnamed_addr global [3 x double] zeroinitializer, align 16
@.str.1 = private unnamed_addr constant [39 x i8] c"Deducted %.2f from %s (new level=%.2f)\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"WATER\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"ELECTRICITY\00", align 1
@.str.4 = private unnamed_addr constant [5 x i8] c"FUEL\00", align 1
@.str.5 = private unnamed_addr constant [54 x i8] c"FAILED deduct: insufficient %s (have %.2f, need %.2f)\00", align 1
@.str.6 = private unnamed_addr constant [51 x i8] c"ERROR: Invalid addResource call (res=%d, amt=%.2f)\00", align 1
@.str.7 = private unnamed_addr constant [34 x i8] c"Added %.2f to %s (new level=%.2f)\00", align 1
@.str.8 = private unnamed_addr constant [65 x i8] c"convert: source and target are the same \E2\80\93 no conversion needed\00", align 1
@.str.9 = private unnamed_addr constant [78 x i8] c"Attempt conversion: %s -> %s : need %.2f of target, requires %.2f from source\00", align 1
@.str.10 = private unnamed_addr constant [72 x i8] c"Conversion successful: consumed %.2f of source, produced %.2f of target\00", align 1
@.str.11 = private unnamed_addr constant [43 x i8] c"Conversion failed during add - rolled back\00", align 1
@.str.12 = private unnamed_addr constant [42 x i8] c"Conversion failed: insufficient source %s\00", align 1
@.str.13 = private unnamed_addr constant [32 x i8] c"ObtainResource: need %.2f of %s\00", align 1
@.str.14 = private unnamed_addr constant [48 x i8] c"ObtainResource succeeded via conversion from %s\00", align 1
@.str.15 = private unnamed_addr constant [67 x i8] c"ObtainResource failed: no conversion path available for %.2f of %s\00", align 1
@.str.16 = private unnamed_addr constant [26 x i8] c"Request to use %.2f of %s\00", align 1
@.str.17 = private unnamed_addr constant [54 x i8] c"Deficit of %.2f, trying to obtain additional resource\00", align 1
@.str.18 = private unnamed_addr constant [65 x i8] c"Cannot use %.2f of %s \E2\80\93 insufficient after conversion attempts\00", align 1
@.str.19 = private unnamed_addr constant [21 x i8] c"Producing %.2f of %s\00", align 1
@.str.20 = private unnamed_addr constant [11 x i8] c"=== %s ===\00", align 1
@.str.21 = private unnamed_addr constant [5 x i8] c"%s:\0A\00", align 1
@.str.22 = private unnamed_addr constant [20 x i8] c"  Water:      %.2f\0A\00", align 1
@.str.23 = private unnamed_addr constant [20 x i8] c"  Electricity:%.2f\0A\00", align 1
@.str.24 = private unnamed_addr constant [20 x i8] c"  Fuel:       %.2f\0A\00", align 1
@.str.25 = private unnamed_addr constant [18 x i8] c"  Total ops:  %d\0A\00", align 1
@.str.26 = private unnamed_addr constant [37 x i8] c"\0A===== EVENT LOG (%d entries) =====\0A\00", align 1
@.str.29 = private unnamed_addr constant [14 x i8] c"Initial State\00", align 1
@.str.34 = private unnamed_addr constant [12 x i8] c"Final State\00", align 1
@.str.35 = private unnamed_addr constant [22 x i8] c"\0ALast error code: %d\0A\00", align 1
@str = private unnamed_addr constant [23 x i8] c"===== END OF LOG =====\00", align 1
@str.36 = private unnamed_addr constant [60 x i8] c"\0A--- Demo 1: Heavy electricity usage (needs conversion) ---\00", align 1
@str.37 = private unnamed_addr constant [63 x i8] c"\0A--- Demo 2: Produce fuel and convert water to electricity ---\00", align 1
@str.38 = private unnamed_addr constant [83 x i8] c"\0A--- Demo 3: Massive water consumption (beyond reserves and conversion limits) ---\00", align 1
@str.39 = private unnamed_addr constant [50 x i8] c"\0A--- Demo 4: Interleaved production and usage ---\00", align 1

; Function Attrs: nounwind uwtable
define dso_local void @logEvent(ptr noundef readonly captures(none) %0, ...) local_unnamed_addr #0 {
  %2 = alloca [1 x %struct.__va_list_tag], align 16
  tail call void @codemarker_alpha(i32 noundef 0) #6
  %3 = load i32, ptr @logCount, align 4, !tbaa !3
  %4 = icmp sgt i32 %3, 99
  br i1 %4, label %12, label %5

5:                                                ; preds = %1
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %2) #6
  call void @llvm.va_start.p0(ptr nonnull %2)
  %6 = load i32, ptr @logCount, align 4, !tbaa !3
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %7
  %9 = call i32 @vsnprintf(ptr noundef nonnull %8, i64 noundef 256, ptr noundef %0, ptr noundef nonnull %2) #6
  call void @llvm.va_end.p0(ptr nonnull %2)
  %10 = load i32, ptr @logCount, align 4, !tbaa !3
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr @logCount, align 4, !tbaa !3
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %2) #6
  br label %12

12:                                               ; preds = %1, %5
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_start.p0(ptr) #2

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @vsnprintf(ptr noundef captures(none), i64 noundef, ptr noundef readonly captures(none), ptr noundef) local_unnamed_addr #3

; Function Attrs: nocallback nofree nosync nounwind willreturn
declare void @llvm.va_end.p0(ptr) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #1

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @deductResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 1) #6
  %3 = icmp ugt i32 %0, 2
  %4 = fcmp olt double %1, 0.000000e+00
  %5 = or i1 %3, %4
  br i1 %5, label %6, label %7

6:                                                ; preds = %2
  store i32 1, ptr @lastError, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 2) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %27

7:                                                ; preds = %2
  tail call void @codemarker_alpha(i32 noundef 3) #6
  %8 = zext nneg i32 %0 to i64
  %9 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %8
  %10 = load double, ptr %9, align 8, !tbaa !7
  %11 = fcmp ult double %10, %1
  br i1 %11, label %21, label %12

12:                                               ; preds = %7
  %13 = fsub double %10, %1
  store double %13, ptr %9, align 8, !tbaa !7
  %14 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %15 = add nsw i32 %14, 1
  store i32 %15, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 4) #6
  %16 = icmp eq i32 %0, 0
  %17 = icmp eq i32 %0, 1
  %18 = select i1 %17, ptr @.str.3, ptr @.str.4
  %19 = select i1 %16, ptr @.str.2, ptr %18
  %20 = load double, ptr %9, align 8, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %19, double noundef %20)
  br label %27

21:                                               ; preds = %7
  tail call void @codemarker_alpha(i32 noundef 5) #6
  %22 = icmp eq i32 %0, 0
  %23 = icmp eq i32 %0, 1
  %24 = select i1 %23, ptr @.str.3, ptr @.str.4
  %25 = select i1 %22, ptr @.str.2, ptr %24
  %26 = load double, ptr %9, align 8, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %25, double noundef %26, double noundef %1)
  br label %27

27:                                               ; preds = %21, %12, %6
  %28 = phi i1 [ false, %6 ], [ true, %12 ], [ false, %21 ]
  ret i1 %28
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @addResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %3 = icmp ult i32 %0, 3
  %4 = fcmp uge double %1, 0.000000e+00
  %5 = and i1 %3, %4
  br i1 %5, label %7, label %6

6:                                                ; preds = %2
  store i32 2, ptr @lastError, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 7) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %0, double noundef %1)
  br label %19

7:                                                ; preds = %2
  %8 = zext nneg i32 %0 to i64
  %9 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %8
  %10 = load double, ptr %9, align 8, !tbaa !7
  %11 = fadd double %1, %10
  store double %11, ptr %9, align 8, !tbaa !7
  %12 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %13 = add nsw i32 %12, 1
  store i32 %13, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %14 = icmp eq i32 %0, 0
  %15 = icmp eq i32 %0, 1
  %16 = select i1 %15, ptr @.str.3, ptr @.str.4
  %17 = select i1 %14, ptr @.str.2, ptr %16
  %18 = load double, ptr %9, align 8, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull %17, double noundef %18)
  br label %19

19:                                               ; preds = %7, %6
  ret i1 %5
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @convertResourceToGetTarget(i32 noundef %0, i32 noundef %1, double noundef %2, double noundef %3) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 9) #6
  %5 = icmp eq i32 %0, %1
  br i1 %5, label %6, label %7

6:                                                ; preds = %4
  tail call void @codemarker_alpha(i32 noundef 10) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.8)
  br label %45

7:                                                ; preds = %4
  tail call void @codemarker_alpha(i32 noundef 11) #6
  %8 = fcmp ugt double %2, 0.000000e+00
  br i1 %8, label %9, label %45

9:                                                ; preds = %7
  tail call void @codemarker_alpha(i32 noundef 12) #6
  %10 = fdiv double %2, %3
  tail call void @codemarker_alpha(i32 noundef 13) #6
  %11 = icmp eq i32 %0, 0
  %12 = icmp eq i32 %0, 1
  %13 = select i1 %12, ptr @.str.3, ptr @.str.4
  %14 = select i1 %11, ptr @.str.2, ptr %13
  %15 = icmp eq i32 %1, 0
  %16 = icmp eq i32 %1, 1
  %17 = select i1 %16, ptr @.str.3, ptr @.str.4
  %18 = select i1 %15, ptr @.str.2, ptr %17
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.9, ptr noundef nonnull %14, ptr noundef nonnull %18, double noundef %2, double noundef %10)
  tail call void @codemarker_alpha(i32 noundef 14) #6
  %19 = tail call zeroext i1 @deductResource(i32 noundef %0, double noundef %10)
  br i1 %19, label %20, label %44

20:                                               ; preds = %9
  tail call void @codemarker_alpha(i32 noundef 15) #6
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %21 = icmp ult i32 %1, 3
  br i1 %21, label %22, label %30

22:                                               ; preds = %20
  %23 = zext nneg i32 %1 to i64
  %24 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %23
  %25 = load double, ptr %24, align 8, !tbaa !7
  %26 = fadd double %2, %25
  store double %26, ptr %24, align 8, !tbaa !7
  %27 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %28 = add nsw i32 %27, 1
  store i32 %28, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %29 = load double, ptr %24, align 8, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %2, ptr noundef nonnull %18, double noundef %29)
  tail call void @codemarker_alpha(i32 noundef 16) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.10, double noundef %10, double noundef %2)
  br label %45

30:                                               ; preds = %20
  store i32 2, ptr @lastError, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 7) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %1, double noundef %2)
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %31 = icmp ult i32 %0, 3
  %32 = fcmp uge double %10, 0.000000e+00
  %33 = and i1 %31, %32
  br i1 %33, label %35, label %34

34:                                               ; preds = %30
  store i32 2, ptr @lastError, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 7) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %0, double noundef %10)
  br label %43

35:                                               ; preds = %30
  %36 = zext nneg i32 %0 to i64
  %37 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %36
  %38 = load double, ptr %37, align 8, !tbaa !7
  %39 = fadd double %10, %38
  store double %39, ptr %37, align 8, !tbaa !7
  %40 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %41 = add nsw i32 %40, 1
  store i32 %41, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %42 = load double, ptr %37, align 8, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %10, ptr noundef nonnull %14, double noundef %42)
  br label %43

43:                                               ; preds = %34, %35
  tail call void @codemarker_alpha(i32 noundef 17) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.11)
  br label %45

44:                                               ; preds = %9
  tail call void @codemarker_alpha(i32 noundef 18) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.12, ptr noundef nonnull %14)
  br label %45

45:                                               ; preds = %22, %43, %44, %7, %6
  %46 = phi i1 [ false, %6 ], [ true, %7 ], [ true, %22 ], [ false, %43 ], [ false, %44 ]
  ret i1 %46
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @obtainResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 19) #6
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %23

4:                                                ; preds = %2
  tail call void @codemarker_alpha(i32 noundef 20) #6
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.13, double noundef %1, ptr noundef nonnull %8)
  tail call void @codemarker_alpha(i32 noundef 21) #6
  tail call void @codemarker_alpha(i32 noundef 22) #6
  br i1 %5, label %9, label %10

9:                                                ; preds = %4
  tail call void @codemarker_alpha(i32 noundef 22) #6
  br label %14

10:                                               ; preds = %4
  tail call void @codemarker_alpha(i32 noundef 23) #6
  %11 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %11, label %21, label %12

12:                                               ; preds = %10
  tail call void @codemarker_alpha(i32 noundef 22) #6
  br i1 %6, label %13, label %14

13:                                               ; preds = %12
  tail call void @codemarker_alpha(i32 noundef 22) #6
  br label %18

14:                                               ; preds = %9, %12
  tail call void @codemarker_alpha(i32 noundef 23) #6
  %15 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %15, label %21, label %16

16:                                               ; preds = %14
  tail call void @codemarker_alpha(i32 noundef 22) #6
  %17 = icmp eq i32 %0, 2
  br i1 %17, label %20, label %18

18:                                               ; preds = %13, %16
  tail call void @codemarker_alpha(i32 noundef 23) #6
  %19 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 2, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %19, label %21, label %20

20:                                               ; preds = %18, %16
  tail call void @codemarker_alpha(i32 noundef 25) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.15, double noundef %1, ptr noundef nonnull %8)
  br label %23

21:                                               ; preds = %18, %14, %10
  %22 = phi ptr [ @.str.2, %10 ], [ @.str.3, %14 ], [ @.str.4, %18 ]
  tail call void @codemarker_alpha(i32 noundef 24) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.14, ptr noundef nonnull %22)
  br label %23

23:                                               ; preds = %21, %2, %20
  %24 = phi i1 [ false, %20 ], [ true, %21 ], [ true, %2 ]
  ret i1 %24
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @useResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 26) #6
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %22

4:                                                ; preds = %2
  tail call void @codemarker_alpha(i32 noundef 27) #6
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.16, double noundef %1, ptr noundef nonnull %8)
  tail call void @codemarker_alpha(i32 noundef 28) #6
  %9 = sext i32 %0 to i64
  %10 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %9
  %11 = load double, ptr %10, align 8, !tbaa !7
  %12 = fcmp ult double %11, %1
  br i1 %12, label %15, label %13

13:                                               ; preds = %4
  %14 = tail call zeroext i1 @deductResource(i32 noundef %0, double noundef %1)
  br label %22

15:                                               ; preds = %4
  tail call void @codemarker_alpha(i32 noundef 29) #6
  %16 = load double, ptr %10, align 8, !tbaa !7
  %17 = fsub double %1, %16
  tail call void @codemarker_alpha(i32 noundef 30) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.17, double noundef %17)
  tail call void @codemarker_alpha(i32 noundef 31) #6
  %18 = tail call zeroext i1 @obtainResource(i32 noundef %0, double noundef %17)
  br i1 %18, label %19, label %21

19:                                               ; preds = %15
  %20 = tail call zeroext i1 @deductResource(i32 noundef %0, double noundef %1)
  br label %22

21:                                               ; preds = %15
  tail call void @codemarker_alpha(i32 noundef 32) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.18, double noundef %1, ptr noundef nonnull %8)
  store i32 3, ptr @lastError, align 4, !tbaa !3
  br label %22

22:                                               ; preds = %19, %21, %2, %13
  %23 = phi i1 [ %14, %13 ], [ true, %2 ], [ %20, %19 ], [ false, %21 ]
  ret i1 %23
}

; Function Attrs: nounwind uwtable
define dso_local noundef zeroext i1 @produceResource(i32 noundef %0, double noundef %1) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 33) #6
  %3 = fcmp olt double %1, 0.000000e+00
  br i1 %3, label %25, label %4

4:                                                ; preds = %2
  tail call void @codemarker_alpha(i32 noundef 34) #6
  %5 = icmp eq i32 %0, 0
  br i1 %5, label %6, label %12

6:                                                ; preds = %4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef %1, ptr noundef nonnull @.str.2)
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %7 = load double, ptr @resources, align 16, !tbaa !7
  %8 = fadd double %1, %7
  store double %8, ptr @resources, align 16, !tbaa !7
  %9 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %11 = load double, ptr @resources, align 16, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull @.str.2, double noundef %11)
  br label %25

12:                                               ; preds = %4
  %13 = icmp eq i32 %0, 1
  %14 = select i1 %13, ptr @.str.3, ptr @.str.4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef %1, ptr noundef nonnull %14)
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %15 = icmp ult i32 %0, 3
  br i1 %15, label %17, label %16

16:                                               ; preds = %12
  store i32 2, ptr @lastError, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 7) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %0, double noundef %1)
  br label %25

17:                                               ; preds = %12
  %18 = zext nneg i32 %0 to i64
  %19 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %18
  %20 = load double, ptr %19, align 8, !tbaa !7
  %21 = fadd double %1, %20
  store double %21, ptr %19, align 8, !tbaa !7
  %22 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %23 = add nsw i32 %22, 1
  store i32 %23, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %24 = load double, ptr %19, align 8, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %1, ptr noundef nonnull %14, double noundef %24)
  br label %25

25:                                               ; preds = %17, %16, %6, %2
  %26 = phi i1 [ false, %2 ], [ true, %6 ], [ false, %16 ], [ true, %17 ]
  ret i1 %26
}

; Function Attrs: nounwind uwtable
define dso_local void @printResources(ptr noundef %0) local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 35) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef %0)
  tail call void @codemarker_alpha(i32 noundef 36) #6
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef %0)
  tail call void @codemarker_alpha(i32 noundef 37) #6
  %3 = load double, ptr @resources, align 16, !tbaa !7
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %3)
  tail call void @codemarker_alpha(i32 noundef 38) #6
  %5 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !7
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %5)
  tail call void @codemarker_alpha(i32 noundef 39) #6
  %7 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !7
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %7)
  tail call void @codemarker_alpha(i32 noundef 40) #6
  %9 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %9)
  ret void
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: nounwind uwtable
define dso_local void @printLogs() local_unnamed_addr #0 {
  tail call void @codemarker_alpha(i32 noundef 41) #6
  %1 = load i32, ptr @logCount, align 4, !tbaa !3
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.26, i32 noundef %1)
  tail call void @codemarker_alpha(i32 noundef 42) #6
  %3 = load i32, ptr @logCount, align 4, !tbaa !3
  %4 = icmp sgt i32 %3, 0
  br i1 %4, label %7, label %5

5:                                                ; preds = %7, %0
  tail call void @codemarker_alpha(i32 noundef 44) #6
  %6 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret void

7:                                                ; preds = %0, %7
  %8 = phi i64 [ %11, %7 ], [ 0, %0 ]
  tail call void @codemarker_alpha(i32 noundef 43) #6
  %9 = getelementptr inbounds nuw [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %8
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) %9)
  %11 = add nuw nsw i64 %8, 1
  %12 = load i32, ptr @logCount, align 4, !tbaa !3
  %13 = sext i32 %12 to i64
  %14 = icmp slt i64 %11, %13
  br i1 %14, label %7, label %5, !llvm.loop !9
}

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #4

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  store <2 x double> <double 1.000000e+02, double 5.000000e+01>, ptr @resources, align 16, !tbaa !7
  store double 8.000000e+01, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !7
  store i32 0, ptr @totalOperations, align 4, !tbaa !3
  store i32 0, ptr @lastError, align 4, !tbaa !3
  store i32 0, ptr @logCount, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 45) #6
  tail call void @printResources(ptr noundef nonnull @.str.29)
  tail call void @codemarker_alpha(i32 noundef 46) #6
  %1 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.36)
  %2 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 7.000000e+01)
  tail call void @codemarker_alpha(i32 noundef 47) #6
  %3 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.37)
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 3.000000e+01, ptr noundef nonnull @.str.4)
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %4 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !7
  %5 = fadd double %4, 3.000000e+01
  store double %5, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !7
  %6 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %7 = add nsw i32 %6, 1
  store i32 %7, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %8 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 3.000000e+01, ptr noundef nonnull @.str.4, double noundef %8)
  %9 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef 1, double noundef 1.500000e+01, double noundef 8.000000e-01)
  tail call void @codemarker_alpha(i32 noundef 48) #6
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.38)
  %11 = tail call zeroext i1 @useResource(i32 noundef 0, double noundef 1.200000e+02)
  tail call void @codemarker_alpha(i32 noundef 49) #6
  %12 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.39)
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 1.000000e+01, ptr noundef nonnull @.str.3)
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %13 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !7
  %14 = fadd double %13, 1.000000e+01
  store double %14, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !7
  %15 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %17 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 1.000000e+01, ptr noundef nonnull @.str.3, double noundef %17)
  %18 = tail call zeroext i1 @useResource(i32 noundef 2, double noundef 5.000000e+01)
  tail call void @codemarker_alpha(i32 noundef 33) #6
  tail call void @codemarker_alpha(i32 noundef 34) #6
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 4.000000e+01, ptr noundef nonnull @.str.2)
  tail call void @codemarker_alpha(i32 noundef 6) #6
  %19 = load double, ptr @resources, align 16, !tbaa !7
  %20 = fadd double %19, 4.000000e+01
  store double %20, ptr @resources, align 16, !tbaa !7
  %21 = load i32, ptr @totalOperations, align 4, !tbaa !3
  %22 = add nsw i32 %21, 1
  store i32 %22, ptr @totalOperations, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 8) #6
  %23 = load double, ptr @resources, align 16, !tbaa !7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 4.000000e+01, ptr noundef nonnull @.str.2, double noundef %23)
  %24 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 2.500000e+01)
  tail call void @codemarker_alpha(i32 noundef 50) #6
  tail call void @printResources(ptr noundef nonnull @.str.34)
  tail call void @codemarker_alpha(i32 noundef 51) #6
  %25 = load i32, ptr @lastError, align 4, !tbaa !3
  %26 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.35, i32 noundef %25)
  tail call void @codemarker_alpha(i32 noundef 52) #6
  tail call void @printLogs()
  ret i32 0
}

; Function Attrs: nofree norecurse nounwind memory(inaccessiblemem: readwrite) uwtable
define dso_local void @codemarker_alpha(i32 noundef %0) local_unnamed_addr #5 {
  %2 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %2)
  store volatile i32 %0, ptr %2, align 4, !tbaa !3
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %2)
  ret void
}

attributes #0 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nocallback nofree nosync nounwind willreturn }
attributes #3 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind }
attributes #5 = { nofree norecurse nounwind memory(inaccessiblemem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nounwind }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = !{!8, !8, i64 0}
!8 = !{!"double", !5, i64 0}
!9 = distinct !{!9, !10}
!10 = !{!"llvm.loop.mustprogress"}
