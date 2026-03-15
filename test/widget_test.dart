// This is a basic Flutter widget test.
//
// To perform an interaction with a widget in your test, use the WidgetTester
// utility in the flutter_test package. For example, you can send tap and scroll
// gestures. You can also use WidgetTester to find child widgets in the widget
// tree, read text, and verify that the values of widget properties are correct.

import 'package:flutter_test/flutter_test.dart';

import 'package:myfirstapp/main.dart';

void main() {
  testWidgets('auth gate renders login screen by default',
      (WidgetTester tester) async {
    await tester.pumpWidget(const PetTrackerApp());

    expect(find.text('Pet Beacon'), findsOneWidget);
    expect(find.text('Sign in to track your pet tag.'), findsOneWidget);
    expect(find.text('Login'), findsWidgets);
    expect(find.text('Register'), findsOneWidget);
  });
}
