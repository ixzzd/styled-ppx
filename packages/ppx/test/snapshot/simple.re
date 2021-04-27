/*
This tests ensure that the ppx transform the right extensions, to the right form.
There's one case for each of the different methods and doesn't need to type-check.

If you are looking to add some tests for CSS support,
check packages/ppx/test/native folder.
*/

[%styled.global {|
  html, body {
    margin: 0;
  }
|}];

module ShoudNotBreakOtherModulesPpxsWithStringAsPayload = [%ppx ""];
module ShoudNotBreakOtherModulesPpxsWithMultiStringAsPayload = [%ppx {| stuff |}];

module OneSingleProperty = [%styled.div "display: block"];
module SingleQuoteStrings = [%styled.section "
  display: flex;
  justify-content: center;
"];

module MultiLineStrings = [%styled.section {|
  display: flex;
  justify-content: center;
|}];

module SelfClosingElement = [%styled.input ""];

let var = "#333333";
module StringInterpolation = [%styled.div {j|
  color: $var;
  __UNSAFE__ color: trust-me;
  display: block;
|j}];

let className = [%cx "display: block;"];
let classNameWithMultiLine = [%cx {| display: block; |}];

let cssRule = [%css "color: blue;"];

module DynamicComponent = [%styled.div
  (~var) => {j|
     color: $var;
     display: block;
   |j}
];