; ModuleID = 'file2_origin_m_clangopt.out.0.2.internalize.bc'
source_filename = "ld-temp.o"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }

@totalOperations = internal global i32 0, align 4
@lastError = internal global i32 0, align 4
@logCount = internal global i32 0, align 4
@logMessages = internal global [100 x [256 x i8]] zeroinitializer, align 16
@.str = private unnamed_addr constant [54 x i8] c"ERROR: Invalid deductResource call (res=%d, amt=%.2f)\00", align 1
@resources = internal global [3 x double] zeroinitializer, align 16
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

; Function Attrs: nofree nounwind uwtable
define internal void @logEvent(ptr noundef readonly captures(none) %0, ...) #0 {
  %2 = alloca [1 x %struct.__va_list_tag], align 16
  %3 = load i32, ptr @logCount, align 4, !tbaa !5
  %4 = icmp sgt i32 %3, 99
  br i1 %4, label %12, label %5

5:                                                ; preds = %1
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %2) #5
  call void @llvm.va_start.p0(ptr nonnull %2)
  %6 = load i32, ptr @logCount, align 4, !tbaa !5
  %7 = sext i32 %6 to i64
  %8 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %7
  %9 = call i32 @vsnprintf(ptr noundef nonnull %8, i64 noundef 256, ptr noundef %0, ptr noundef nonnull %2) #5
  call void @llvm.va_end.p0(ptr nonnull %2)
  %10 = load i32, ptr @logCount, align 4, !tbaa !5
  %11 = add nsw i32 %10, 1
  store i32 %11, ptr @logCount, align 4, !tbaa !5
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %2) #5
  br label %12

12:                                               ; preds = %5, %1
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

; Function Attrs: nofree nounwind uwtable
define internal noundef zeroext i1 @convertResourceToGetTarget(i32 noundef %0, i32 noundef %1, double noundef %2, double noundef %3) #0 {
  %5 = icmp eq i32 %0, %1
  br i1 %5, label %6, label %7

6:                                                ; preds = %4
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.8)
  br label %47

7:                                                ; preds = %4
  %8 = fcmp ugt double %2, 0.000000e+00
  br i1 %8, label %9, label %47

9:                                                ; preds = %7
  %10 = fdiv double %2, %3
  %11 = icmp eq i32 %0, 0
  %12 = icmp eq i32 %0, 1
  %13 = select i1 %12, ptr @.str.3, ptr @.str.4
  %14 = select i1 %11, ptr @.str.2, ptr %13
  %15 = icmp eq i32 %1, 0
  %16 = icmp eq i32 %1, 1
  %17 = select i1 %16, ptr @.str.3, ptr @.str.4
  %18 = select i1 %15, ptr @.str.2, ptr %17
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.9, ptr noundef nonnull %14, ptr noundef nonnull %18, double noundef %2, double noundef %10)
  %19 = icmp ugt i32 %0, 2
  %20 = fcmp olt double %10, 0.000000e+00
  %21 = or i1 %19, %20
  br i1 %21, label %22, label %23

22:                                               ; preds = %9
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %10)
  br label %46

23:                                               ; preds = %9
  %24 = zext nneg i32 %0 to i64
  %25 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %24
  %26 = load double, ptr %25, align 8, !tbaa !9
  %27 = fcmp ult double %26, %10
  br i1 %27, label %28, label %29

28:                                               ; preds = %23
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %14, double noundef %26, double noundef %10)
  br label %46

29:                                               ; preds = %23
  %30 = fsub double %26, %10
  store double %30, ptr %25, align 8, !tbaa !9
  %31 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %32 = add nsw i32 %31, 1
  store i32 %32, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %10, ptr noundef nonnull %14, double noundef %30)
  %33 = icmp ult i32 %1, 3
  br i1 %33, label %34, label %41

34:                                               ; preds = %29
  %35 = zext nneg i32 %1 to i64
  %36 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %35
  %37 = load double, ptr %36, align 8, !tbaa !9
  %38 = fadd double %2, %37
  store double %38, ptr %36, align 8, !tbaa !9
  %39 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %40 = add nsw i32 %39, 1
  store i32 %40, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %2, ptr noundef nonnull %18, double noundef %38)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.10, double noundef %10, double noundef %2)
  br label %47

41:                                               ; preds = %29
  store i32 2, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.6, i32 noundef %1, double noundef %2)
  %42 = load double, ptr %25, align 8, !tbaa !9
  %43 = fadd double %10, %42
  store double %43, ptr %25, align 8, !tbaa !9
  %44 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %45 = add nsw i32 %44, 1
  store i32 %45, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %10, ptr noundef nonnull %14, double noundef %43)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.11)
  br label %47

46:                                               ; preds = %28, %22
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.12, ptr noundef nonnull %14)
  br label %47

47:                                               ; preds = %46, %41, %34, %7, %6
  %48 = phi i1 [ false, %6 ], [ true, %7 ], [ true, %34 ], [ false, %41 ], [ false, %46 ]
  ret i1 %48
}

; Function Attrs: nofree nounwind uwtable
define internal noundef zeroext i1 @obtainResource(i32 noundef %0, double noundef %1) #0 {
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %23

4:                                                ; preds = %2
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.13, double noundef %1, ptr noundef nonnull %8)
  br i1 %5, label %14, label %9

9:                                                ; preds = %4
  %10 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %10, label %21, label %11

11:                                               ; preds = %9
  br i1 %6, label %18, label %12

12:                                               ; preds = %11
  %13 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %13, label %21, label %16

14:                                               ; preds = %4
  %15 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %15, label %21, label %18

16:                                               ; preds = %12
  %17 = icmp eq i32 %0, 2
  br i1 %17, label %20, label %18

18:                                               ; preds = %16, %14, %11
  %19 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 2, i32 noundef %0, double noundef %1, double noundef 8.000000e-01)
  br i1 %19, label %21, label %20

20:                                               ; preds = %18, %16
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.15, double noundef %1, ptr noundef nonnull %8)
  br label %23

21:                                               ; preds = %18, %14, %12, %9
  %22 = phi ptr [ @.str.2, %9 ], [ @.str.3, %12 ], [ @.str.4, %18 ], [ @.str.3, %14 ]
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.14, ptr noundef nonnull %22)
  br label %23

23:                                               ; preds = %21, %20, %2
  %24 = phi i1 [ false, %20 ], [ true, %21 ], [ true, %2 ]
  ret i1 %24
}

; Function Attrs: nofree nounwind uwtable
define internal noundef zeroext i1 @useResource(i32 noundef %0, double noundef %1) #0 {
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %39

4:                                                ; preds = %2
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.16, double noundef %1, ptr noundef nonnull %8)
  %9 = sext i32 %0 to i64
  %10 = getelementptr inbounds [3 x double], ptr @resources, i64 0, i64 %9
  %11 = load double, ptr %10, align 8, !tbaa !9
  %12 = fcmp ult double %11, %1
  br i1 %12, label %22, label %13

13:                                               ; preds = %4
  %14 = icmp ugt i32 %0, 2
  br i1 %14, label %15, label %16

15:                                               ; preds = %13
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %39

16:                                               ; preds = %13
  %17 = zext nneg i32 %0 to i64
  %18 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %17
  %19 = fsub double %11, %1
  store double %19, ptr %18, align 8, !tbaa !9
  %20 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %21 = add nsw i32 %20, 1
  store i32 %21, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %8, double noundef %19)
  br label %39

22:                                               ; preds = %4
  %23 = fsub double %1, %11
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.17, double noundef %23)
  %24 = tail call zeroext i1 @obtainResource(i32 noundef %0, double noundef %23)
  br i1 %24, label %25, label %38

25:                                               ; preds = %22
  %26 = icmp ugt i32 %0, 2
  br i1 %26, label %27, label %28

27:                                               ; preds = %25
  store i32 1, ptr @lastError, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str, i32 noundef %0, double noundef %1)
  br label %39

28:                                               ; preds = %25
  %29 = zext nneg i32 %0 to i64
  %30 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %29
  %31 = load double, ptr %30, align 8, !tbaa !9
  %32 = fcmp ult double %31, %1
  br i1 %32, label %37, label %33

33:                                               ; preds = %28
  %34 = fsub double %31, %1
  store double %34, ptr %30, align 8, !tbaa !9
  %35 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %36 = add nsw i32 %35, 1
  store i32 %36, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %8, double noundef %34)
  br label %39

37:                                               ; preds = %28
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %8, double noundef %31, double noundef %1)
  br label %39

38:                                               ; preds = %22
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.18, double noundef %1, ptr noundef nonnull %8)
  store i32 3, ptr @lastError, align 4, !tbaa !5
  br label %39

39:                                               ; preds = %38, %37, %33, %27, %16, %15, %2
  %40 = phi i1 [ true, %2 ], [ false, %38 ], [ false, %15 ], [ true, %16 ], [ false, %27 ], [ true, %33 ], [ false, %37 ]
  ret i1 %40
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef i32 @main() #0 {
  store <2 x double> <double 1.000000e+02, double 5.000000e+01>, ptr @resources, align 16, !tbaa !9
  store double 8.000000e+01, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  store i32 0, ptr @totalOperations, align 4, !tbaa !5
  store i32 0, ptr @lastError, align 4, !tbaa !5
  store i32 0, ptr @logCount, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef nonnull @.str.29)
  %1 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef nonnull @.str.29)
  %2 = load double, ptr @resources, align 16, !tbaa !9
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %2)
  %4 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %4)
  %6 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %6)
  %8 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %8)
  %10 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.36)
  %11 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 7.000000e+01)
  %12 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.37)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 3.000000e+01, ptr noundef nonnull @.str.4)
  %13 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %14 = fadd double %13, 3.000000e+01
  store double %14, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %15 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 3.000000e+01, ptr noundef nonnull @.str.4, double noundef %14)
  %17 = tail call zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef 1, double noundef 1.500000e+01, double noundef 8.000000e-01)
  %18 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.38)
  %19 = tail call zeroext i1 @useResource(i32 noundef 0, double noundef 1.200000e+02)
  %20 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.39)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 1.000000e+01, ptr noundef nonnull @.str.3)
  %21 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %22 = fadd double %21, 1.000000e+01
  store double %22, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %23 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %24 = add nsw i32 %23, 1
  store i32 %24, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 1.000000e+01, ptr noundef nonnull @.str.3, double noundef %22)
  %25 = tail call zeroext i1 @useResource(i32 noundef 2, double noundef 5.000000e+01)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 4.000000e+01, ptr noundef nonnull @.str.2)
  %26 = load double, ptr @resources, align 16, !tbaa !9
  %27 = fadd double %26, 4.000000e+01
  store double %27, ptr @resources, align 16, !tbaa !9
  %28 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %29 = add nsw i32 %28, 1
  store i32 %29, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 4.000000e+01, ptr noundef nonnull @.str.2, double noundef %27)
  %30 = tail call zeroext i1 @useResource(i32 noundef 1, double noundef 2.500000e+01)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef nonnull @.str.34)
  %31 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef nonnull @.str.34)
  %32 = load double, ptr @resources, align 16, !tbaa !9
  %33 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %32)
  %34 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %35 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %34)
  %36 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %37 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %36)
  %38 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %39 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %38)
  %40 = load i32, ptr @lastError, align 4, !tbaa !5
  %41 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.35, i32 noundef %40)
  %42 = load i32, ptr @logCount, align 4, !tbaa !5
  %43 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.26, i32 noundef %42)
  %44 = load i32, ptr @logCount, align 4, !tbaa !5
  %45 = icmp sgt i32 %44, 0
  br i1 %45, label %46, label %54

46:                                               ; preds = %46, %0
  %47 = phi i64 [ %50, %46 ], [ 0, %0 ]
  %48 = getelementptr inbounds nuw [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %47
  %49 = tail call i32 @puts(ptr nonnull dereferenceable(1) %48)
  %50 = add nuw nsw i64 %47, 1
  %51 = load i32, ptr @logCount, align 4, !tbaa !5
  %52 = sext i32 %51 to i64
  %53 = icmp slt i64 %50, %52
  br i1 %53, label %46, label %54, !llvm.loop !11

54:                                               ; preds = %46, %0
  %55 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: nofree nounwind
declare noundef i32 @puts(ptr noundef readonly captures(none)) local_unnamed_addr #4

attributes #0 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { nocallback nofree nosync nounwind willreturn }
attributes #3 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { nofree nounwind }
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
!9 = !{!10, !10, i64 0}
!10 = !{!"double", !7, i64 0}
!11 = distinct !{!11, !12}
!12 = !{!"llvm.loop.mustprogress"}
