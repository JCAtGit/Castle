; ModuleID = 'file2_origin_m_clangopt.out.0.5.precodegen.bc'
source_filename = "ld-temp.o"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

%struct.__va_list_tag = type { i32, i32, ptr, ptr }

@totalOperations = internal unnamed_addr global i32 0, align 4
@lastError = internal unnamed_addr global i1 false, align 4
@logCount = internal unnamed_addr global i32 0, align 4
@logMessages = internal global [100 x [256 x i8]] zeroinitializer, align 16
@resources = internal unnamed_addr global [3 x double] zeroinitializer, align 16
@.str.1 = private unnamed_addr constant [39 x i8] c"Deducted %.2f from %s (new level=%.2f)\00", align 1
@.str.2 = private unnamed_addr constant [6 x i8] c"WATER\00", align 1
@.str.3 = private unnamed_addr constant [12 x i8] c"ELECTRICITY\00", align 1
@.str.4 = private unnamed_addr constant [5 x i8] c"FUEL\00", align 1
@.str.5 = private unnamed_addr constant [54 x i8] c"FAILED deduct: insufficient %s (have %.2f, need %.2f)\00", align 1
@.str.7 = private unnamed_addr constant [34 x i8] c"Added %.2f to %s (new level=%.2f)\00", align 1
@.str.8 = private unnamed_addr constant [65 x i8] c"convert: source and target are the same \E2\80\93 no conversion needed\00", align 1
@.str.9 = private unnamed_addr constant [78 x i8] c"Attempt conversion: %s -> %s : need %.2f of target, requires %.2f from source\00", align 1
@.str.10 = private unnamed_addr constant [72 x i8] c"Conversion successful: consumed %.2f of source, produced %.2f of target\00", align 1
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
define internal void @logEvent(ptr noundef readonly captures(none) %0, ...) unnamed_addr #0 {
  %2 = alloca [1 x %struct.__va_list_tag], align 16
  %3 = load i32, ptr @logCount, align 4, !tbaa !5
  %4 = icmp sgt i32 %3, 99
  br i1 %4, label %11, label %5

5:                                                ; preds = %1
  call void @llvm.lifetime.start.p0(i64 24, ptr nonnull %2) #5
  call void @llvm.va_start.p0(ptr nonnull %2)
  %6 = sext i32 %3 to i64
  %7 = getelementptr inbounds [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %6
  %8 = call i32 @vsnprintf(ptr noundef nonnull %7, i64 noundef 256, ptr noundef %0, ptr noundef nonnull %2) #5
  call void @llvm.va_end.p0(ptr nonnull %2)
  %9 = load i32, ptr @logCount, align 4, !tbaa !5
  %10 = add nsw i32 %9, 1
  store i32 %10, ptr @logCount, align 4, !tbaa !5
  call void @llvm.lifetime.end.p0(i64 24, ptr nonnull %2) #5
  br label %11

11:                                               ; preds = %5, %1
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
define internal fastcc noundef zeroext i1 @convertResourceToGetTarget(i32 noundef range(i32 0, 3) %0, i32 noundef range(i32 0, 3) %1, double noundef %2) unnamed_addr #0 {
  %4 = icmp eq i32 %0, %1
  br i1 %4, label %5, label %6

5:                                                ; preds = %3
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.8)
  br label %33

6:                                                ; preds = %3
  %7 = fcmp ugt double %2, 0.000000e+00
  br i1 %7, label %8, label %33

8:                                                ; preds = %6
  %9 = fdiv double %2, 8.000000e-01
  %10 = icmp eq i32 %0, 0
  %11 = icmp eq i32 %0, 1
  %12 = select i1 %11, ptr @.str.3, ptr @.str.4
  %13 = select i1 %10, ptr @.str.2, ptr %12
  %14 = icmp eq i32 %1, 0
  %15 = icmp eq i32 %1, 1
  %16 = select i1 %15, ptr @.str.3, ptr @.str.4
  %17 = select i1 %14, ptr @.str.2, ptr %16
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.9, ptr noundef nonnull %13, ptr noundef nonnull %17, double noundef %2, double noundef %9)
  %18 = zext nneg i32 %0 to i64
  %19 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %18
  %20 = load double, ptr %19, align 8, !tbaa !9
  %21 = fcmp ult double %20, %9
  br i1 %21, label %32, label %22

22:                                               ; preds = %8
  %23 = fsub double %20, %9
  store double %23, ptr %19, align 8, !tbaa !9
  %24 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %25 = add nsw i32 %24, 1
  store i32 %25, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %9, ptr noundef nonnull %13, double noundef %23)
  %26 = zext nneg i32 %1 to i64
  %27 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %26
  %28 = load double, ptr %27, align 8, !tbaa !9
  %29 = fadd double %2, %28
  store double %29, ptr %27, align 8, !tbaa !9
  %30 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %31 = add nsw i32 %30, 1
  store i32 %31, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef %2, ptr noundef nonnull %17, double noundef %29)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.10, double noundef %9, double noundef %2)
  br label %33

32:                                               ; preds = %8
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %13, double noundef %20, double noundef %9)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.12, ptr noundef nonnull %13)
  br label %33

33:                                               ; preds = %32, %22, %6, %5
  %34 = phi i1 [ false, %5 ], [ true, %6 ], [ true, %22 ], [ false, %32 ]
  ret i1 %34
}

; Function Attrs: nofree nounwind uwtable
define internal fastcc void @useResource(i32 noundef range(i32 0, 3) %0, double noundef %1) unnamed_addr #0 {
  %3 = fcmp ugt double %1, 0.000000e+00
  br i1 %3, label %4, label %41

4:                                                ; preds = %2
  %5 = icmp eq i32 %0, 0
  %6 = icmp eq i32 %0, 1
  %7 = select i1 %6, ptr @.str.3, ptr @.str.4
  %8 = select i1 %5, ptr @.str.2, ptr %7
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.16, double noundef %1, ptr noundef nonnull %8)
  %9 = zext nneg i32 %0 to i64
  %10 = getelementptr inbounds nuw [3 x double], ptr @resources, i64 0, i64 %9
  %11 = load double, ptr %10, align 8, !tbaa !9
  %12 = fcmp ult double %11, %1
  br i1 %12, label %17, label %13

13:                                               ; preds = %4
  %14 = fsub double %11, %1
  store double %14, ptr %10, align 8, !tbaa !9
  %15 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %16 = add nsw i32 %15, 1
  store i32 %16, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %8, double noundef %14)
  br label %41

17:                                               ; preds = %4
  %18 = fsub double %1, %11
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.17, double noundef %18)
  %19 = fcmp ugt double %18, 0.000000e+00
  br i1 %19, label %20, label %32

20:                                               ; preds = %17
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.13, double noundef %18, ptr noundef nonnull %8)
  br i1 %5, label %26, label %21

21:                                               ; preds = %20
  %22 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef range(i32 0, 3) %0, double noundef %18)
  br i1 %22, label %30, label %23

23:                                               ; preds = %21
  br i1 %6, label %28, label %24

24:                                               ; preds = %23
  %25 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef range(i32 0, 3) %0, double noundef %18)
  br i1 %25, label %30, label %40

26:                                               ; preds = %20
  %27 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 1, i32 noundef 0, double noundef %18)
  br i1 %27, label %30, label %28

28:                                               ; preds = %26, %23
  %29 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 2, i32 noundef range(i32 0, 3) %0, double noundef %18)
  br i1 %29, label %30, label %40

30:                                               ; preds = %28, %26, %24, %21
  %31 = phi ptr [ @.str.2, %21 ], [ @.str.3, %24 ], [ @.str.4, %28 ], [ @.str.3, %26 ]
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.14, ptr noundef nonnull %31)
  br label %32

32:                                               ; preds = %30, %17
  %33 = load double, ptr %10, align 8, !tbaa !9
  %34 = fcmp ult double %33, %1
  br i1 %34, label %39, label %35

35:                                               ; preds = %32
  %36 = fsub double %33, %1
  store double %36, ptr %10, align 8, !tbaa !9
  %37 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %38 = add nsw i32 %37, 1
  store i32 %38, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.1, double noundef %1, ptr noundef nonnull %8, double noundef %36)
  br label %41

39:                                               ; preds = %32
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.5, ptr noundef nonnull %8, double noundef %33, double noundef %1)
  br label %41

40:                                               ; preds = %28, %24
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.15, double noundef %18, ptr noundef nonnull %8)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.18, double noundef %1, ptr noundef nonnull %8)
  store i1 true, ptr @lastError, align 4
  br label %41

41:                                               ; preds = %40, %39, %35, %13, %2
  ret void
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #0 {
  store <2 x double> <double 1.000000e+02, double 5.000000e+01>, ptr @resources, align 16, !tbaa !9
  store double 8.000000e+01, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  store i32 0, ptr @totalOperations, align 4, !tbaa !5
  store i1 false, ptr @lastError, align 4
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
  tail call fastcc void @useResource(i32 noundef 1, double noundef 7.000000e+01)
  %11 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.37)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 3.000000e+01, ptr noundef nonnull @.str.4)
  %12 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %13 = fadd double %12, 3.000000e+01
  store double %13, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %14 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %15 = add nsw i32 %14, 1
  store i32 %15, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 3.000000e+01, ptr noundef nonnull @.str.4, double noundef %13)
  %16 = tail call fastcc zeroext i1 @convertResourceToGetTarget(i32 noundef 0, i32 noundef 1, double noundef 1.500000e+01)
  %17 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.38)
  tail call fastcc void @useResource(i32 noundef 0, double noundef 1.200000e+02)
  %18 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str.39)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 1.000000e+01, ptr noundef nonnull @.str.3)
  %19 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %20 = fadd double %19, 1.000000e+01
  store double %20, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %21 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %22 = add nsw i32 %21, 1
  store i32 %22, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 1.000000e+01, ptr noundef nonnull @.str.3, double noundef %20)
  tail call fastcc void @useResource(i32 noundef 2, double noundef 5.000000e+01)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.19, double noundef 4.000000e+01, ptr noundef nonnull @.str.2)
  %23 = load double, ptr @resources, align 16, !tbaa !9
  %24 = fadd double %23, 4.000000e+01
  store double %24, ptr @resources, align 16, !tbaa !9
  %25 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %26 = add nsw i32 %25, 1
  store i32 %26, ptr @totalOperations, align 4, !tbaa !5
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.7, double noundef 4.000000e+01, ptr noundef nonnull @.str.2, double noundef %24)
  tail call fastcc void @useResource(i32 noundef 1, double noundef 2.500000e+01)
  tail call void (ptr, ...) @logEvent(ptr noundef nonnull @.str.20, ptr noundef nonnull @.str.34)
  %27 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.21, ptr noundef nonnull @.str.34)
  %28 = load double, ptr @resources, align 16, !tbaa !9
  %29 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.22, double noundef %28)
  %30 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 8), align 8, !tbaa !9
  %31 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.23, double noundef %30)
  %32 = load double, ptr getelementptr inbounds nuw (i8, ptr @resources, i64 16), align 16, !tbaa !9
  %33 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.24, double noundef %32)
  %34 = load i32, ptr @totalOperations, align 4, !tbaa !5
  %35 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.25, i32 noundef %34)
  %.b = load i1, ptr @lastError, align 4
  %36 = select i1 %.b, i32 3, i32 0
  %37 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.35, i32 noundef %36)
  %38 = load i32, ptr @logCount, align 4, !tbaa !5
  %39 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.26, i32 noundef %38)
  %40 = load i32, ptr @logCount, align 4, !tbaa !5
  %41 = icmp sgt i32 %40, 0
  br i1 %41, label %.preheader, label %.loopexit

.preheader:                                       ; preds = %.preheader, %0
  %42 = phi i64 [ %45, %.preheader ], [ 0, %0 ]
  %43 = getelementptr inbounds nuw [100 x [256 x i8]], ptr @logMessages, i64 0, i64 %42
  %44 = tail call i32 @puts(ptr nonnull dereferenceable(1) %43)
  %45 = add nuw nsw i64 %42, 1
  %46 = load i32, ptr @logCount, align 4, !tbaa !5
  %47 = sext i32 %46 to i64
  %48 = icmp slt i64 %45, %47
  br i1 %48, label %.preheader, label %.loopexit, !llvm.loop !11

.loopexit:                                        ; preds = %.preheader, %0
  %49 = tail call i32 @puts(ptr nonnull dereferenceable(1) @str)
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
