; ModuleID = './file4_origin_0_LLVMIR_LINK_PREOPT/./file4_origin_0_LLVMIR_LINK_PREOPT.bc'
source_filename = "llvm-link"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-redhat-linux-gnu"

@.str = private unnamed_addr constant [27 x i8] c"compute_something(5) = %d\0A\00", align 1
@.str.1 = private unnamed_addr constant [22 x i8] c"constant_fold() = %d\0A\00", align 1
@.str.2 = private unnamed_addr constant [28 x i8] c"branch_elimination(0) = %d\0A\00", align 1
@.str.3 = private unnamed_addr constant [23 x i8] c"cse_example(3,4) = %d\0A\00", align 1
@.str.4 = private unnamed_addr constant [30 x i8] c"inline_candidate(10,20) = %d\0A\00", align 1
@.str.5 = private unnamed_addr constant [19 x i8] c"max(100,200) = %d\0A\00", align 1
@.str.6 = private unnamed_addr constant [16 x i8] c"grade(85) = %c\0A\00", align 1
@.str.7 = private unnamed_addr constant [20 x i8] c"algebraic(42) = %d\0A\00", align 1
@.str.8 = private unnamed_addr constant [20 x i8] c"bit_tricks(7) = %d\0A\00", align 1
@__const.main.src = private unnamed_addr constant [32 x i8] c"Hello optimizable world!\00\00\00\00\00\00\00\00", align 16
@.str.9 = private unnamed_addr constant [12 x i8] c"Copied: %s\0A\00", align 1
@.str.10 = private unnamed_addr constant [50 x i8] c"square = %d, add_five = %d, multiply_by_two = %d\0A\00", align 1

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef range(i32 0, -1) i32 @multiply_by_two(i32 noundef %0) local_unnamed_addr #0 {
  %2 = shl i32 %0, 1
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local void @init_array(ptr noundef writeonly captures(none) %0, i32 noundef %1) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 0) #7
  %3 = icmp sgt i32 %1, 0
  br i1 %3, label %4, label %24

4:                                                ; preds = %2
  %5 = zext nneg i32 %1 to i64
  %6 = icmp ult i32 %1, 8
  br i1 %6, label %22, label %7

7:                                                ; preds = %4
  %8 = and i64 %5, 2147483640
  br label %9

9:                                                ; preds = %9, %7
  %10 = phi i64 [ 0, %7 ], [ %17, %9 ]
  %11 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %7 ], [ %18, %9 ]
  %12 = getelementptr inbounds nuw i32, ptr %0, i64 %10
  %13 = mul <4 x i32> %11, splat (i32 3)
  %14 = mul <4 x i32> %11, splat (i32 3)
  %15 = add <4 x i32> %14, splat (i32 12)
  %16 = getelementptr inbounds nuw i8, ptr %12, i64 16
  store <4 x i32> %13, ptr %12, align 4, !tbaa !3
  store <4 x i32> %15, ptr %16, align 4, !tbaa !3
  %17 = add nuw i64 %10, 8
  %18 = add <4 x i32> %11, splat (i32 8)
  %19 = icmp eq i64 %17, %8
  br i1 %19, label %20, label %9, !llvm.loop !7

20:                                               ; preds = %9
  %21 = icmp eq i64 %8, %5
  br i1 %21, label %24, label %22

22:                                               ; preds = %4, %20
  %23 = phi i64 [ 0, %4 ], [ %8, %20 ]
  br label %25

24:                                               ; preds = %25, %20, %2
  ret void

25:                                               ; preds = %22, %25
  %26 = phi i64 [ %30, %25 ], [ %23, %22 ]
  %27 = getelementptr inbounds nuw i32, ptr %0, i64 %26
  %28 = trunc i64 %26 to i32
  %29 = mul i32 %28, 3
  store i32 %29, ptr %27, align 4, !tbaa !3
  %30 = add nuw nsw i64 %26, 1
  %31 = icmp eq i64 %30, %5
  br i1 %31, label %24, label %25, !llvm.loop !11
}

; Function Attrs: nounwind uwtable
define dso_local range(i32 -2147483648, 2147483645) i32 @compute_something(i32 noundef %0) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 1) #7
  %2 = shl i32 %0, 1
  tail call void @codemarker_alpha(i32 noundef 2) #7
  %3 = add i32 %2, 17
  ret i32 %3
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @constant_fold() local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 3) #7
  tail call void @codemarker_alpha(i32 noundef 4) #7
  tail call void @codemarker_alpha(i32 noundef 5) #7
  tail call void @codemarker_alpha(i32 noundef 6) #7
  tail call void @codemarker_alpha(i32 noundef 7) #7
  ret i32 20
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @branch_elimination(i32 noundef %0) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 8) #7
  tail call void @codemarker_alpha(i32 noundef 9) #7
  tail call void @codemarker_alpha(i32 noundef 10) #7
  ret i32 48
}

; Function Attrs: nounwind uwtable
define dso_local range(i32 -2147483648, 2147483647) i32 @cse_example(i32 noundef %0, i32 noundef %1) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 11) #7
  tail call void @codemarker_alpha(i32 noundef 12) #7
  %3 = shl i32 %0, 1
  %4 = mul i32 %3, %1
  ret i32 %4
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local range(i32 -2147483548, -2147483648) i32 @inline_candidate(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = tail call noundef i32 @llvm.smax.i32(i32 %0, i32 %1)
  %4 = add nsw i32 %3, 100
  ret i32 %4
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #2

; Function Attrs: nounwind uwtable
define dso_local i32 @loop_invariant(ptr noundef readonly captures(none) %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 13) #7
  tail call void @codemarker_alpha(i32 noundef 14) #7
  %4 = icmp sgt i32 %1, 0
  br i1 %4, label %5, label %33

5:                                                ; preds = %3
  %6 = zext nneg i32 %1 to i64
  %7 = icmp ult i32 %1, 8
  br i1 %7, label %30, label %8

8:                                                ; preds = %5
  %9 = and i64 %6, 2147483640
  %10 = insertelement <4 x i32> poison, i32 %2, i64 0
  %11 = shufflevector <4 x i32> %10, <4 x i32> poison, <4 x i32> zeroinitializer
  br label %12

12:                                               ; preds = %12, %8
  %13 = phi i64 [ 0, %8 ], [ %24, %12 ]
  %14 = phi <4 x i32> [ zeroinitializer, %8 ], [ %22, %12 ]
  %15 = phi <4 x i32> [ zeroinitializer, %8 ], [ %23, %12 ]
  %16 = getelementptr inbounds nuw i32, ptr %0, i64 %13
  %17 = getelementptr inbounds nuw i8, ptr %16, i64 16
  %18 = load <4 x i32>, ptr %16, align 4, !tbaa !3
  %19 = load <4 x i32>, ptr %17, align 4, !tbaa !3
  %20 = mul nsw <4 x i32> %18, %11
  %21 = mul nsw <4 x i32> %19, %11
  %22 = add <4 x i32> %20, %14
  %23 = add <4 x i32> %21, %15
  %24 = add nuw i64 %13, 8
  %25 = icmp eq i64 %24, %9
  br i1 %25, label %26, label %12, !llvm.loop !12

26:                                               ; preds = %12
  %27 = add <4 x i32> %23, %22
  %28 = tail call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %27)
  %29 = icmp eq i64 %9, %6
  br i1 %29, label %33, label %30

30:                                               ; preds = %5, %26
  %31 = phi i64 [ 0, %5 ], [ %9, %26 ]
  %32 = phi i32 [ 0, %5 ], [ %28, %26 ]
  br label %35

33:                                               ; preds = %35, %26, %3
  %34 = phi i32 [ 0, %3 ], [ %28, %26 ], [ %41, %35 ]
  ret i32 %34

35:                                               ; preds = %30, %35
  %36 = phi i64 [ %42, %35 ], [ %31, %30 ]
  %37 = phi i32 [ %41, %35 ], [ %32, %30 ]
  %38 = getelementptr inbounds nuw i32, ptr %0, i64 %36
  %39 = load i32, ptr %38, align 4, !tbaa !3
  %40 = mul nsw i32 %39, %2
  %41 = add nsw i32 %40, %37
  %42 = add nuw nsw i64 %36, 1
  %43 = icmp eq i64 %42, %6
  br i1 %43, label %33, label %35, !llvm.loop !13
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.add.v4i32(<4 x i32>) #2

; Function Attrs: nounwind uwtable
define dso_local void @strength_reduce(ptr noundef writeonly captures(none) %0, i32 noundef %1) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 15) #7
  tail call void @codemarker_alpha(i32 noundef 16) #7
  %3 = icmp sgt i32 %1, 0
  br i1 %3, label %4, label %25

4:                                                ; preds = %2
  %5 = zext nneg i32 %1 to i64
  %6 = icmp ult i32 %1, 8
  br i1 %6, label %22, label %7

7:                                                ; preds = %4
  %8 = and i64 %5, 2147483640
  %9 = trunc nuw nsw i64 %8 to i32
  %10 = shl nuw i32 %9, 1
  br label %11

11:                                               ; preds = %11, %7
  %12 = phi i64 [ 0, %7 ], [ %17, %11 ]
  %13 = phi <4 x i32> [ <i32 0, i32 2, i32 4, i32 6>, %7 ], [ %18, %11 ]
  %14 = add <4 x i32> %13, splat (i32 8)
  %15 = getelementptr inbounds nuw i32, ptr %0, i64 %12
  %16 = getelementptr inbounds nuw i8, ptr %15, i64 16
  store <4 x i32> %13, ptr %15, align 4, !tbaa !3
  store <4 x i32> %14, ptr %16, align 4, !tbaa !3
  %17 = add nuw i64 %12, 8
  %18 = add <4 x i32> %13, splat (i32 16)
  %19 = icmp eq i64 %17, %8
  br i1 %19, label %20, label %11, !llvm.loop !14

20:                                               ; preds = %11
  %21 = icmp eq i64 %8, %5
  br i1 %21, label %25, label %22

22:                                               ; preds = %4, %20
  %23 = phi i64 [ 0, %4 ], [ %8, %20 ]
  %24 = phi i32 [ 0, %4 ], [ %10, %20 ]
  br label %26

25:                                               ; preds = %26, %20, %2
  ret void

26:                                               ; preds = %22, %26
  %27 = phi i64 [ %31, %26 ], [ %23, %22 ]
  %28 = phi i32 [ %30, %26 ], [ %24, %22 ]
  %29 = getelementptr inbounds nuw i32, ptr %0, i64 %27
  store i32 %28, ptr %29, align 4, !tbaa !3
  %30 = add nuw nsw i32 %28, 2
  %31 = add nuw nsw i64 %27, 1
  %32 = icmp eq i64 %31, %5
  br i1 %32, label %25, label %26, !llvm.loop !15
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local i32 @dead_code(i32 noundef %0) local_unnamed_addr #0 {
  %2 = mul nsw i32 %0, %0
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local i32 @store_load(ptr noundef captures(none) initializes((0, 4)) %0) local_unnamed_addr #1 {
  store i32 10, ptr %0, align 4, !tbaa !3
  tail call void @codemarker_alpha(i32 noundef 18) #7
  %2 = load i32, ptr %0, align 4, !tbaa !3
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @max(i32 noundef %0, i32 noundef %1) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 19) #7
  %3 = tail call i32 @llvm.smax.i32(i32 %0, i32 %1)
  ret i32 %3
}

; Function Attrs: nounwind uwtable
define dso_local signext range(i8 65, 71) i8 @grade(i32 noundef %0) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 20) #7
  %2 = sdiv i32 %0, 10
  switch i32 %2, label %7 [
    i32 10, label %3
    i32 9, label %8
    i32 8, label %4
    i32 7, label %5
    i32 6, label %6
  ]

3:                                                ; preds = %1
  tail call void @codemarker_alpha(i32 noundef 22) #7
  br label %8

4:                                                ; preds = %1
  br label %8

5:                                                ; preds = %1
  br label %8

6:                                                ; preds = %1
  br label %8

7:                                                ; preds = %1
  br label %8

8:                                                ; preds = %3, %1, %7, %6, %5, %4
  %9 = phi i8 [ 70, %7 ], [ 66, %4 ], [ 67, %5 ], [ 68, %6 ], [ 65, %1 ], [ 65, %3 ]
  ret i8 %9
}

; Function Attrs: nounwind uwtable
define dso_local void @copy_buffer(ptr noundef writeonly captures(none) initializes((0, 32)) %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 26) #7
  tail call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 1 dereferenceable(32) %0, ptr noundef nonnull align 1 dereferenceable(32) %1, i64 32, i1 false)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #3

; Function Attrs: nounwind uwtable
define dso_local void @loop_fusion(ptr noundef captures(none) %0, ptr noundef writeonly captures(none) %1, i32 noundef %2) local_unnamed_addr #1 {
  %4 = ptrtoint ptr %0 to i64
  %5 = ptrtoint ptr %1 to i64
  tail call void @codemarker_alpha(i32 noundef 27) #7
  %6 = icmp sgt i32 %2, 0
  br i1 %6, label %7, label %25

7:                                                ; preds = %3
  %8 = zext nneg i32 %2 to i64
  %9 = icmp ult i32 %2, 8
  br i1 %9, label %23, label %10

10:                                               ; preds = %7
  %11 = and i64 %8, 2147483640
  br label %12

12:                                               ; preds = %12, %10
  %13 = phi i64 [ 0, %10 ], [ %18, %12 ]
  %14 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %10 ], [ %19, %12 ]
  %15 = add <4 x i32> %14, splat (i32 4)
  %16 = getelementptr inbounds nuw i32, ptr %0, i64 %13
  %17 = getelementptr inbounds nuw i8, ptr %16, i64 16
  store <4 x i32> %14, ptr %16, align 4, !tbaa !3
  store <4 x i32> %15, ptr %17, align 4, !tbaa !3
  %18 = add nuw i64 %13, 8
  %19 = add <4 x i32> %14, splat (i32 8)
  %20 = icmp eq i64 %18, %11
  br i1 %20, label %21, label %12, !llvm.loop !16

21:                                               ; preds = %12
  %22 = icmp eq i64 %11, %8
  br i1 %22, label %26, label %23

23:                                               ; preds = %7, %21
  %24 = phi i64 [ 0, %7 ], [ %11, %21 ]
  br label %66

25:                                               ; preds = %3
  tail call void @codemarker_alpha(i32 noundef 28) #7
  br label %72

26:                                               ; preds = %66, %21
  tail call void @codemarker_alpha(i32 noundef 28) #7
  %27 = zext nneg i32 %2 to i64
  %28 = icmp ult i32 %2, 8
  %29 = sub i64 %5, %4
  %30 = icmp ult i64 %29, 32
  %31 = or i1 %28, %30
  br i1 %31, label %48, label %32

32:                                               ; preds = %26
  %33 = and i64 %8, 2147483640
  br label %34

34:                                               ; preds = %34, %32
  %35 = phi i64 [ 0, %32 ], [ %44, %34 ]
  %36 = getelementptr inbounds nuw i32, ptr %0, i64 %35
  %37 = getelementptr inbounds nuw i8, ptr %36, i64 16
  %38 = load <4 x i32>, ptr %36, align 4, !tbaa !3
  %39 = load <4 x i32>, ptr %37, align 4, !tbaa !3
  %40 = shl nsw <4 x i32> %38, splat (i32 1)
  %41 = shl nsw <4 x i32> %39, splat (i32 1)
  %42 = getelementptr inbounds nuw i32, ptr %1, i64 %35
  %43 = getelementptr inbounds nuw i8, ptr %42, i64 16
  store <4 x i32> %40, ptr %42, align 4, !tbaa !3
  store <4 x i32> %41, ptr %43, align 4, !tbaa !3
  %44 = add nuw i64 %35, 8
  %45 = icmp eq i64 %44, %33
  br i1 %45, label %46, label %34, !llvm.loop !17

46:                                               ; preds = %34
  %47 = icmp eq i64 %33, %8
  br i1 %47, label %72, label %48

48:                                               ; preds = %26, %46
  %49 = phi i64 [ 0, %26 ], [ %33, %46 ]
  %50 = and i64 %8, 3
  %51 = icmp eq i64 %50, 0
  br i1 %51, label %62, label %52

52:                                               ; preds = %48, %52
  %53 = phi i64 [ %59, %52 ], [ %49, %48 ]
  %54 = phi i64 [ %60, %52 ], [ 0, %48 ]
  %55 = getelementptr inbounds nuw i32, ptr %0, i64 %53
  %56 = load i32, ptr %55, align 4, !tbaa !3
  %57 = shl nsw i32 %56, 1
  %58 = getelementptr inbounds nuw i32, ptr %1, i64 %53
  store i32 %57, ptr %58, align 4, !tbaa !3
  %59 = add nuw nsw i64 %53, 1
  %60 = add i64 %54, 1
  %61 = icmp eq i64 %60, %50
  br i1 %61, label %62, label %52, !llvm.loop !18

62:                                               ; preds = %52, %48
  %63 = phi i64 [ %49, %48 ], [ %59, %52 ]
  %64 = sub nsw i64 %49, %8
  %65 = icmp ugt i64 %64, -4
  br i1 %65, label %72, label %73

66:                                               ; preds = %23, %66
  %67 = phi i64 [ %70, %66 ], [ %24, %23 ]
  %68 = getelementptr inbounds nuw i32, ptr %0, i64 %67
  %69 = trunc nuw nsw i64 %67 to i32
  store i32 %69, ptr %68, align 4, !tbaa !3
  %70 = add nuw nsw i64 %67, 1
  %71 = icmp eq i64 %70, %8
  br i1 %71, label %26, label %66, !llvm.loop !20

72:                                               ; preds = %62, %73, %46, %25
  ret void

73:                                               ; preds = %62, %73
  %74 = phi i64 [ %94, %73 ], [ %63, %62 ]
  %75 = getelementptr inbounds nuw i32, ptr %0, i64 %74
  %76 = load i32, ptr %75, align 4, !tbaa !3
  %77 = shl nsw i32 %76, 1
  %78 = getelementptr inbounds nuw i32, ptr %1, i64 %74
  store i32 %77, ptr %78, align 4, !tbaa !3
  %79 = add nuw nsw i64 %74, 1
  %80 = getelementptr inbounds nuw i32, ptr %0, i64 %79
  %81 = load i32, ptr %80, align 4, !tbaa !3
  %82 = shl nsw i32 %81, 1
  %83 = getelementptr inbounds nuw i32, ptr %1, i64 %79
  store i32 %82, ptr %83, align 4, !tbaa !3
  %84 = add nuw nsw i64 %74, 2
  %85 = getelementptr inbounds nuw i32, ptr %0, i64 %84
  %86 = load i32, ptr %85, align 4, !tbaa !3
  %87 = shl nsw i32 %86, 1
  %88 = getelementptr inbounds nuw i32, ptr %1, i64 %84
  store i32 %87, ptr %88, align 4, !tbaa !3
  %89 = add nuw nsw i64 %74, 3
  %90 = getelementptr inbounds nuw i32, ptr %0, i64 %89
  %91 = load i32, ptr %90, align 4, !tbaa !3
  %92 = shl nsw i32 %91, 1
  %93 = getelementptr inbounds nuw i32, ptr %1, i64 %89
  store i32 %92, ptr %93, align 4, !tbaa !3
  %94 = add nuw nsw i64 %74, 4
  %95 = icmp eq i64 %94, %27
  br i1 %95, label %72, label %73, !llvm.loop !21
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @algebraic(i32 noundef %0) local_unnamed_addr #0 {
  ret i32 3
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @bit_tricks(i32 noundef %0) local_unnamed_addr #0 {
  %2 = mul i32 %0, 6
  ret i32 %2
}

; Function Attrs: nounwind uwtable
define dso_local void @empty_loop(i32 noundef %0) local_unnamed_addr #1 {
  tail call void @codemarker_alpha(i32 noundef 29) #7
  ret void
}

; Function Attrs: nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #1 {
  %1 = alloca [32 x i8], align 16
  tail call void @codemarker_alpha(i32 noundef 30) #7
  tail call void @codemarker_alpha(i32 noundef 31) #7
  tail call void @codemarker_alpha(i32 noundef 0) #7
  tail call void @codemarker_alpha(i32 noundef 32) #7
  tail call void @codemarker_alpha(i32 noundef 1) #7
  tail call void @codemarker_alpha(i32 noundef 2) #7
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef 27)
  tail call void @codemarker_alpha(i32 noundef 33) #7
  tail call void @codemarker_alpha(i32 noundef 3) #7
  tail call void @codemarker_alpha(i32 noundef 4) #7
  tail call void @codemarker_alpha(i32 noundef 5) #7
  tail call void @codemarker_alpha(i32 noundef 6) #7
  tail call void @codemarker_alpha(i32 noundef 7) #7
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1, i32 noundef 20)
  tail call void @codemarker_alpha(i32 noundef 34) #7
  tail call void @codemarker_alpha(i32 noundef 8) #7
  tail call void @codemarker_alpha(i32 noundef 9) #7
  tail call void @codemarker_alpha(i32 noundef 10) #7
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2, i32 noundef 48)
  tail call void @codemarker_alpha(i32 noundef 35) #7
  tail call void @codemarker_alpha(i32 noundef 11) #7
  tail call void @codemarker_alpha(i32 noundef 12) #7
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.3, i32 noundef 24)
  tail call void @codemarker_alpha(i32 noundef 36) #7
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.4, i32 noundef 120)
  tail call void @codemarker_alpha(i32 noundef 37) #7
  tail call void @codemarker_alpha(i32 noundef 19) #7
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.5, i32 noundef 200)
  tail call void @codemarker_alpha(i32 noundef 38) #7
  tail call void @codemarker_alpha(i32 noundef 20) #7
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.6, i32 noundef 66)
  tail call void @codemarker_alpha(i32 noundef 39) #7
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.7, i32 noundef 3)
  tail call void @codemarker_alpha(i32 noundef 40) #7
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.8, i32 noundef 42)
  tail call void @codemarker_alpha(i32 noundef 41) #7
  tail call void @codemarker_alpha(i32 noundef 42) #7
  call void @llvm.lifetime.start.p0(i64 32, ptr nonnull %1) #7
  tail call void @codemarker_alpha(i32 noundef 43) #7
  tail call void @codemarker_alpha(i32 noundef 26) #7
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 16 dereferenceable(32) %1, ptr noundef nonnull readonly align 16 dereferenceable(32) @__const.main.src, i64 32, i1 false)
  tail call void @codemarker_alpha(i32 noundef 44) #7
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.9, ptr noundef nonnull %1)
  call void @codemarker_alpha(i32 noundef 45) #7
  call void @codemarker_alpha(i32 noundef 46) #7
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.10, i32 noundef 100, i32 noundef 25, i32 noundef 20)
  call void @llvm.lifetime.end.p0(i64 32, ptr nonnull %1) #7
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #5

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #5

; Function Attrs: nofree norecurse nounwind memory(inaccessiblemem: readwrite) uwtable
define dso_local void @codemarker_alpha(i32 noundef %0) local_unnamed_addr #6 {
  %2 = alloca i32, align 4
  call void @llvm.lifetime.start.p0(i64 4, ptr nonnull %2)
  store volatile i32 %0, ptr %2, align 4, !tbaa !3
  call void @llvm.lifetime.end.p0(i64 4, ptr nonnull %2)
  ret void
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #4 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #6 = { nofree norecurse nounwind memory(inaccessiblemem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #7 = { nounwind }

!llvm.ident = !{!0, !0}
!llvm.module.flags = !{!1, !2}

!0 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{!4, !4, i64 0}
!4 = !{!"int", !5, i64 0}
!5 = !{!"omnipotent char", !6, i64 0}
!6 = !{!"Simple C/C++ TBAA"}
!7 = distinct !{!7, !8, !9, !10}
!8 = !{!"llvm.loop.mustprogress"}
!9 = !{!"llvm.loop.isvectorized", i32 1}
!10 = !{!"llvm.loop.unroll.runtime.disable"}
!11 = distinct !{!11, !8, !10, !9}
!12 = distinct !{!12, !8, !9, !10}
!13 = distinct !{!13, !8, !10, !9}
!14 = distinct !{!14, !8, !9, !10}
!15 = distinct !{!15, !8, !10, !9}
!16 = distinct !{!16, !8, !9, !10}
!17 = distinct !{!17, !8, !9, !10}
!18 = distinct !{!18, !19}
!19 = !{!"llvm.loop.unroll.disable"}
!20 = distinct !{!20, !8, !10, !9}
!21 = distinct !{!21, !8, !9}
