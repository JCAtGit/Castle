; ModuleID = './file4_origin_m_PREOPT/file4_origin.bc'
source_filename = "file4_origin.c"
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

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: write) uwtable
define dso_local void @init_array(ptr noundef writeonly captures(none) %0, i32 noundef %1) local_unnamed_addr #1 {
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
  store <4 x i32> %13, ptr %12, align 4, !tbaa !5
  store <4 x i32> %15, ptr %16, align 4, !tbaa !5
  %17 = add nuw i64 %10, 8
  %18 = add <4 x i32> %11, splat (i32 8)
  %19 = icmp eq i64 %17, %8
  br i1 %19, label %20, label %9, !llvm.loop !9

20:                                               ; preds = %9
  %21 = icmp eq i64 %8, %5
  br i1 %21, label %24, label %22

22:                                               ; preds = %20, %4
  %23 = phi i64 [ 0, %4 ], [ %8, %20 ]
  br label %25

24:                                               ; preds = %25, %20, %2
  ret void

25:                                               ; preds = %25, %22
  %26 = phi i64 [ %30, %25 ], [ %23, %22 ]
  %27 = getelementptr inbounds nuw i32, ptr %0, i64 %26
  %28 = trunc i64 %26 to i32
  %29 = mul i32 %28, 3
  store i32 %29, ptr %27, align 4, !tbaa !5
  %30 = add nuw nsw i64 %26, 1
  %31 = icmp eq i64 %30, %5
  br i1 %31, label %24, label %25, !llvm.loop !13
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.start.p0(i64 immarg, ptr captures(none)) #2

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.lifetime.end.p0(i64 immarg, ptr captures(none)) #2

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local range(i32 -2147483648, 2147483645) i32 @compute_something(i32 noundef %0) local_unnamed_addr #0 {
  %2 = shl i32 %0, 1
  %3 = add i32 %2, 17
  ret i32 %3
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @constant_fold() local_unnamed_addr #0 {
  ret i32 20
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @branch_elimination(i32 noundef %0) local_unnamed_addr #0 {
  ret i32 48
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local range(i32 -2147483648, 2147483647) i32 @cse_example(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
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

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: read) uwtable
define dso_local i32 @loop_invariant(ptr noundef readonly captures(none) %0, i32 noundef %1, i32 noundef %2) local_unnamed_addr #3 {
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
  %18 = load <4 x i32>, ptr %16, align 4, !tbaa !5
  %19 = load <4 x i32>, ptr %17, align 4, !tbaa !5
  %20 = mul nsw <4 x i32> %18, %11
  %21 = mul nsw <4 x i32> %19, %11
  %22 = add <4 x i32> %20, %14
  %23 = add <4 x i32> %21, %15
  %24 = add nuw i64 %13, 8
  %25 = icmp eq i64 %24, %9
  br i1 %25, label %26, label %12, !llvm.loop !14

26:                                               ; preds = %12
  %27 = add <4 x i32> %23, %22
  %28 = tail call i32 @llvm.vector.reduce.add.v4i32(<4 x i32> %27)
  %29 = icmp eq i64 %9, %6
  br i1 %29, label %33, label %30

30:                                               ; preds = %26, %5
  %31 = phi i64 [ 0, %5 ], [ %9, %26 ]
  %32 = phi i32 [ 0, %5 ], [ %28, %26 ]
  br label %35

33:                                               ; preds = %35, %26, %3
  %34 = phi i32 [ 0, %3 ], [ %28, %26 ], [ %41, %35 ]
  ret i32 %34

35:                                               ; preds = %35, %30
  %36 = phi i64 [ %42, %35 ], [ %31, %30 ]
  %37 = phi i32 [ %41, %35 ], [ %32, %30 ]
  %38 = getelementptr inbounds nuw i32, ptr %0, i64 %36
  %39 = load i32, ptr %38, align 4, !tbaa !5
  %40 = mul nsw i32 %39, %2
  %41 = add nsw i32 %40, %37
  %42 = add nuw nsw i64 %36, 1
  %43 = icmp eq i64 %42, %6
  br i1 %43, label %33, label %35, !llvm.loop !15
}

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: write) uwtable
define dso_local void @strength_reduce(ptr noundef writeonly captures(none) %0, i32 noundef %1) local_unnamed_addr #1 {
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
  store <4 x i32> %13, ptr %15, align 4, !tbaa !5
  store <4 x i32> %14, ptr %16, align 4, !tbaa !5
  %17 = add nuw i64 %12, 8
  %18 = add <4 x i32> %13, splat (i32 16)
  %19 = icmp eq i64 %17, %8
  br i1 %19, label %20, label %11, !llvm.loop !16

20:                                               ; preds = %11
  %21 = icmp eq i64 %8, %5
  br i1 %21, label %25, label %22

22:                                               ; preds = %20, %4
  %23 = phi i64 [ 0, %4 ], [ %8, %20 ]
  %24 = phi i32 [ 0, %4 ], [ %10, %20 ]
  br label %26

25:                                               ; preds = %26, %20, %2
  ret void

26:                                               ; preds = %26, %22
  %27 = phi i64 [ %31, %26 ], [ %23, %22 ]
  %28 = phi i32 [ %30, %26 ], [ %24, %22 ]
  %29 = getelementptr inbounds nuw i32, ptr %0, i64 %27
  store i32 %28, ptr %29, align 4, !tbaa !5
  %30 = add nuw nsw i32 %28, 2
  %31 = add nuw nsw i64 %27, 1
  %32 = icmp eq i64 %31, %5
  br i1 %32, label %25, label %26, !llvm.loop !17
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local i32 @dead_code(i32 noundef %0) local_unnamed_addr #0 {
  %2 = mul nsw i32 %0, %0
  ret i32 %2
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) uwtable
define dso_local noundef i32 @store_load(ptr noundef writeonly captures(none) initializes((0, 4)) %0) local_unnamed_addr #4 {
  store i32 10, ptr %0, align 4, !tbaa !5
  ret i32 10
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local noundef i32 @max(i32 noundef %0, i32 noundef %1) local_unnamed_addr #0 {
  %3 = tail call i32 @llvm.smax.i32(i32 %0, i32 %1)
  ret i32 %3
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local signext range(i8 65, 71) i8 @grade(i32 noundef %0) local_unnamed_addr #0 {
  %2 = sdiv i32 %0, 10
  %3 = add nsw i32 %2, -6
  %4 = icmp ult i32 %3, 5
  %5 = zext i32 %3 to i40
  %6 = shl nuw nsw i40 %5, 3
  %7 = lshr i40 280267735876, %6
  %8 = trunc i40 %7 to i8
  %9 = select i1 %4, i8 %8, i8 70
  ret i8 %9
}

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) uwtable
define dso_local void @copy_buffer(ptr noundef writeonly captures(none) initializes((0, 32)) %0, ptr noundef readonly captures(none) %1) local_unnamed_addr #5 {
  tail call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 1 dereferenceable(32) %0, ptr noundef nonnull align 1 dereferenceable(32) %1, i64 32, i1 false)
  ret void
}

; Function Attrs: nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #6

; Function Attrs: nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable
define dso_local void @loop_fusion(ptr noundef captures(none) %0, ptr noundef writeonly captures(none) %1, i32 noundef %2) local_unnamed_addr #7 {
  %4 = ptrtoint ptr %0 to i64
  %5 = ptrtoint ptr %1 to i64
  %6 = icmp sgt i32 %2, 0
  br i1 %6, label %7, label %71

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
  store <4 x i32> %14, ptr %16, align 4, !tbaa !5
  store <4 x i32> %15, ptr %17, align 4, !tbaa !5
  %18 = add nuw i64 %13, 8
  %19 = add <4 x i32> %14, splat (i32 8)
  %20 = icmp eq i64 %18, %11
  br i1 %20, label %21, label %12, !llvm.loop !18

21:                                               ; preds = %12
  %22 = icmp eq i64 %11, %8
  br i1 %22, label %25, label %23

23:                                               ; preds = %21, %7
  %24 = phi i64 [ 0, %7 ], [ %11, %21 ]
  br label %65

25:                                               ; preds = %65, %21
  %26 = zext nneg i32 %2 to i64
  %27 = icmp ult i32 %2, 8
  %28 = sub i64 %5, %4
  %29 = icmp ult i64 %28, 32
  %30 = or i1 %27, %29
  br i1 %30, label %47, label %31

31:                                               ; preds = %25
  %32 = and i64 %8, 2147483640
  br label %33

33:                                               ; preds = %33, %31
  %34 = phi i64 [ 0, %31 ], [ %43, %33 ]
  %35 = getelementptr inbounds nuw i32, ptr %0, i64 %34
  %36 = getelementptr inbounds nuw i8, ptr %35, i64 16
  %37 = load <4 x i32>, ptr %35, align 4, !tbaa !5
  %38 = load <4 x i32>, ptr %36, align 4, !tbaa !5
  %39 = shl nsw <4 x i32> %37, splat (i32 1)
  %40 = shl nsw <4 x i32> %38, splat (i32 1)
  %41 = getelementptr inbounds nuw i32, ptr %1, i64 %34
  %42 = getelementptr inbounds nuw i8, ptr %41, i64 16
  store <4 x i32> %39, ptr %41, align 4, !tbaa !5
  store <4 x i32> %40, ptr %42, align 4, !tbaa !5
  %43 = add nuw i64 %34, 8
  %44 = icmp eq i64 %43, %32
  br i1 %44, label %45, label %33, !llvm.loop !19

45:                                               ; preds = %33
  %46 = icmp eq i64 %32, %8
  br i1 %46, label %71, label %47

47:                                               ; preds = %45, %25
  %48 = phi i64 [ 0, %25 ], [ %32, %45 ]
  %49 = and i64 %8, 3
  %50 = icmp eq i64 %49, 0
  br i1 %50, label %61, label %51

51:                                               ; preds = %51, %47
  %52 = phi i64 [ %58, %51 ], [ %48, %47 ]
  %53 = phi i64 [ %59, %51 ], [ 0, %47 ]
  %54 = getelementptr inbounds nuw i32, ptr %0, i64 %52
  %55 = load i32, ptr %54, align 4, !tbaa !5
  %56 = shl nsw i32 %55, 1
  %57 = getelementptr inbounds nuw i32, ptr %1, i64 %52
  store i32 %56, ptr %57, align 4, !tbaa !5
  %58 = add nuw nsw i64 %52, 1
  %59 = add i64 %53, 1
  %60 = icmp eq i64 %59, %49
  br i1 %60, label %61, label %51, !llvm.loop !20

61:                                               ; preds = %51, %47
  %62 = phi i64 [ %48, %47 ], [ %58, %51 ]
  %63 = sub nsw i64 %48, %8
  %64 = icmp ugt i64 %63, -4
  br i1 %64, label %71, label %72

65:                                               ; preds = %65, %23
  %66 = phi i64 [ %69, %65 ], [ %24, %23 ]
  %67 = getelementptr inbounds nuw i32, ptr %0, i64 %66
  %68 = trunc nuw nsw i64 %66 to i32
  store i32 %68, ptr %67, align 4, !tbaa !5
  %69 = add nuw nsw i64 %66, 1
  %70 = icmp eq i64 %69, %8
  br i1 %70, label %25, label %65, !llvm.loop !22

71:                                               ; preds = %72, %61, %45, %3
  ret void

72:                                               ; preds = %72, %61
  %73 = phi i64 [ %93, %72 ], [ %62, %61 ]
  %74 = getelementptr inbounds nuw i32, ptr %0, i64 %73
  %75 = load i32, ptr %74, align 4, !tbaa !5
  %76 = shl nsw i32 %75, 1
  %77 = getelementptr inbounds nuw i32, ptr %1, i64 %73
  store i32 %76, ptr %77, align 4, !tbaa !5
  %78 = add nuw nsw i64 %73, 1
  %79 = getelementptr inbounds nuw i32, ptr %0, i64 %78
  %80 = load i32, ptr %79, align 4, !tbaa !5
  %81 = shl nsw i32 %80, 1
  %82 = getelementptr inbounds nuw i32, ptr %1, i64 %78
  store i32 %81, ptr %82, align 4, !tbaa !5
  %83 = add nuw nsw i64 %73, 2
  %84 = getelementptr inbounds nuw i32, ptr %0, i64 %83
  %85 = load i32, ptr %84, align 4, !tbaa !5
  %86 = shl nsw i32 %85, 1
  %87 = getelementptr inbounds nuw i32, ptr %1, i64 %83
  store i32 %86, ptr %87, align 4, !tbaa !5
  %88 = add nuw nsw i64 %73, 3
  %89 = getelementptr inbounds nuw i32, ptr %0, i64 %88
  %90 = load i32, ptr %89, align 4, !tbaa !5
  %91 = shl nsw i32 %90, 1
  %92 = getelementptr inbounds nuw i32, ptr %1, i64 %88
  store i32 %91, ptr %92, align 4, !tbaa !5
  %93 = add nuw nsw i64 %73, 4
  %94 = icmp eq i64 %93, %26
  br i1 %94, label %71, label %72, !llvm.loop !23
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

; Function Attrs: mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable
define dso_local void @empty_loop(i32 noundef %0) local_unnamed_addr #0 {
  ret void
}

; Function Attrs: nofree nounwind uwtable
define dso_local noundef i32 @main() local_unnamed_addr #8 {
  %1 = alloca [32 x i8], align 16
  %2 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str, i32 noundef 27)
  %3 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.1, i32 noundef 20)
  %4 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.2, i32 noundef 48)
  %5 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.3, i32 noundef 24)
  %6 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.4, i32 noundef 120)
  %7 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.5, i32 noundef 200)
  %8 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.6, i32 noundef 66)
  %9 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.7, i32 noundef 3)
  %10 = tail call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.8, i32 noundef 42)
  call void @llvm.lifetime.start.p0(i64 32, ptr nonnull %1) #11
  call void @llvm.memcpy.p0.p0.i64(ptr noundef nonnull align 16 dereferenceable(32) %1, ptr noundef nonnull readonly align 16 dereferenceable(32) @__const.main.src, i64 32, i1 false)
  %11 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.9, ptr noundef nonnull %1)
  %12 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @.str.10, i32 noundef 100, i32 noundef 25, i32 noundef 20)
  call void @llvm.lifetime.end.p0(i64 32, ptr nonnull %1) #11
  ret i32 0
}

; Function Attrs: nofree nounwind
declare dso_local noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #9

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #10

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.vector.reduce.add.v4i32(<4 x i32>) #10

attributes #0 = { mustprogress nofree norecurse nosync nounwind willreturn memory(none) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #1 = { nofree norecurse nosync nounwind memory(argmem: write) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #2 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { nofree norecurse nosync nounwind memory(argmem: read) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #4 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: write) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #5 = { mustprogress nofree norecurse nosync nounwind willreturn memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #6 = { nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #7 = { nofree norecurse nosync nounwind memory(argmem: readwrite) uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #8 = { nofree nounwind uwtable "min-legal-vector-width"="0" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #9 = { nofree nounwind "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cmov,+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" }
attributes #10 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #11 = { nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"uwtable", i32 2}
!2 = !{i32 1, !"ThinLTO", i32 0}
!3 = !{i32 1, !"EnableSplitLTOUnit", i32 1}
!4 = !{!"clang version 21.1.8 (Fedora 21.1.8-4.fc43)"}
!5 = !{!6, !6, i64 0}
!6 = !{!"int", !7, i64 0}
!7 = !{!"omnipotent char", !8, i64 0}
!8 = !{!"Simple C/C++ TBAA"}
!9 = distinct !{!9, !10, !11, !12}
!10 = !{!"llvm.loop.mustprogress"}
!11 = !{!"llvm.loop.isvectorized", i32 1}
!12 = !{!"llvm.loop.unroll.runtime.disable"}
!13 = distinct !{!13, !10, !12, !11}
!14 = distinct !{!14, !10, !11, !12}
!15 = distinct !{!15, !10, !12, !11}
!16 = distinct !{!16, !10, !11, !12}
!17 = distinct !{!17, !10, !12, !11}
!18 = distinct !{!18, !10, !11, !12}
!19 = distinct !{!19, !10, !11, !12}
!20 = distinct !{!20, !21}
!21 = !{!"llvm.loop.unroll.disable"}
!22 = distinct !{!22, !10, !12, !11}
!23 = distinct !{!23, !10, !11}

^0 = module: (path: "./file4_origin_m_PREOPT/file4_origin.bc", hash: (0, 0, 0, 0, 0))
^1 = gv: (name: "algebraic", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 1, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 736499631867688647
^2 = gv: (name: ".str.8", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 1530094498781262953
^3 = gv: (name: "copy_buffer", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 1966500351621579433
^4 = gv: (name: ".str.7", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 2259290126537346677
^5 = gv: (name: "llvm.memcpy.p0.p0.i64") ; guid = 2699563562842851132
^6 = gv: (name: "inline_candidate", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 3, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 2990092464636197017
^7 = gv: (name: "llvm.lifetime.start.p0") ; guid = 4736283418132453162
^8 = gv: (name: "bit_tricks", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 4887551538650224416
^9 = gv: (name: ".str.3", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 5633919400532139758
^10 = gv: (name: "llvm.smax.i32") ; guid = 6002034434922093760
^11 = gv: (name: "dead_code", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 6257192332597860212
^12 = gv: (name: ".str.9", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 6477107446441723275
^13 = gv: (name: "printf") ; guid = 7383291119112528047
^14 = gv: (name: "max", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 8185957744030711343
^15 = gv: (name: "cse_example", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 3, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 8453375148749674536
^16 = gv: (name: "multiply_by_two", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 8477806741152019488
^17 = gv: (name: "branch_elimination", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 1, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 8699142124250108312
^18 = gv: (name: ".str.5", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 9149013450228509752
^19 = gv: (name: ".str.2", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 9652801601657790934
^20 = gv: (name: "constant_fold", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 1, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 9767906317069561782
^21 = gv: (name: ".str.6", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 9973172625557952486
^22 = gv: (name: "__const.main.src", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10560701773776491305
^23 = gv: (name: ".str", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 10615243208080064493
^24 = gv: (name: "init_array", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 33, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 11372478009472378006
^25 = gv: (name: ".str.1", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12349168911816387147
^26 = gv: (name: ".str.10", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 12371651202119254851
^27 = gv: (name: "empty_loop", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 1, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 12714221607190294795
^28 = gv: (name: "loop_invariant", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 41, funcFlags: (readNone: 0, readOnly: 1, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 12987039436391562778
^29 = gv: (name: "llvm.lifetime.end.p0") ; guid = 13244677477227836793
^30 = gv: (name: "loop_fusion", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 102, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 14397824925514798483
^31 = gv: (name: "compute_something", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 3, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 15258918510218081799
^32 = gv: (name: "store_load", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 2, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 15261083965881353416
^33 = gv: (name: "main", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 16, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 0, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0), calls: ((callee: ^13, tail: 1)), refs: (^23, ^25, ^19, ^9, ^37, ^18, ^21, ^4, ^2, ^22, ^12, ^26)))) ; guid = 15822663052811949562
^34 = gv: (name: "llvm.vector.reduce.add.v4i32") ; guid = 16711401006643563310
^35 = gv: (name: "strength_reduce", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 34, funcFlags: (readNone: 0, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 17890632957303120142
^36 = gv: (name: "grade", summaries: (function: (module: ^0, flags: (linkage: external, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), insts: 9, funcFlags: (readNone: 1, readOnly: 0, noRecurse: 1, returnDoesNotAlias: 0, noInline: 0, alwaysInline: 0, noUnwind: 1, mayThrow: 0, hasUnknownCall: 0, mustBeUnreachable: 0)))) ; guid = 18101706878112262878
^37 = gv: (name: ".str.4", summaries: (variable: (module: ^0, flags: (linkage: private, visibility: default, notEligibleToImport: 1, live: 0, dsoLocal: 1, canAutoHide: 0, importType: definition), varFlags: (readonly: 1, writeonly: 0, constant: 1)))) ; guid = 18267160176698062867
^38 = flags: 8
^39 = blockcount: 0
