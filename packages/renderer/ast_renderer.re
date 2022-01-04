open Css_types;

let render_record = zippedRecord => {
  let inner =
    zippedRecord
    |> List.map(((key, value)) => Printf.sprintf("  %s: %s", key, value))
    |> String.concat(",\n");

  Printf.sprintf("{\n%s\n}", inner);
};

let dimension_of_string =
  fun
  | Length => "Length"
  | Angle => "Angle"
  | Time => "Time"
  | Frequency => "Frequency";

let rec render_stylesheet = (ast: Stylesheet.t) => {
  let inner = ast |> fst |> List.map(render_rule) |> String.concat(", ");
  "Stylesheet([" ++ inner ++ "])";
}
and render_rule = (ast: Rule.t) => {
  switch (ast) {
  | Style_rule(style_rule) => render_style_rule(style_rule)
  | At_rule(at_rule) => render_at_rule(at_rule)
  };
}
and render_style_rule = (ast: Style_rule.t) => {
  render_record([
    ("prelude", ast.prelude |> render_declaration_value),
    ("block", render_declaration_list(ast.block)),
  ]);
}
and render_at_rule = (ast: At_rule.t) => {
  render_record([
    ("prelude", ast.prelude |> render_declaration_value),
    ("block", render_brace_block(ast.block)),
  ]);
}
and render_brace_block = ast => {
  switch ((ast: Brace_block.t)) {
  | Empty => "Empty"
  | Declaration_list(declaration_list) =>
    render_declaration_list(declaration_list)
  | Stylesheet(stylesheet) => render_stylesheet(stylesheet)
  };
}
and render_declaration_kind = (ast: Declaration_list.kind) => {
  switch (ast) {
  | Declaration(declaration) => render_declaration(declaration)
  | Unsafe(unsafe) => render_declaration(unsafe)
  | Style_rule(style_rule) => render_style_rule(style_rule)
  | At_rule(at_rule) => render_at_rule(at_rule)
  };
}
and render_declaration_list = (ast: Declaration_list.t) => {
  let inner =
    ast |> fst |> List.map(render_declaration_kind) |> String.concat(", ");
  "Declaration([" ++ inner ++ "])";
}
and render_declaration = (ast: Declaration.t) => {
  render_record([
    ("name", ast.name |> fst),
    ("value", ast.value |> render_declaration_value),
    ("important", ast.important |> fst |> string_of_bool),
  ]);
}
and render_declaration_value =
    (ast: with_loc(list(with_loc(Component_value.t)))) => {
  let inner =
    ast |> fst |> List.map(render_component_value) |> String.concat(", ");
  "[" ++ inner ++ "]";
}
and render_component_value = (ast: with_loc(Component_value.t)) => {
  let value = ast |> fst;
  switch (value) {
  | Paren_block(block) =>
    let block =
      block |> List.map(render_component_value) |> String.concat(", ");
    "Paren_block(" ++ block ++ ")";
  | Bracket_block(block) =>
    let block =
      block |> List.map(render_component_value) |> String.concat(", ");
    "Bracket_block(" ++ block ++ ")";
  | Percentage(string) => "Percentage(" ++ string ++ ")"
  | Ident(string) => "Ident(" ++ string ++ ")"
  | String(string) => "String(" ++ string ++ ")"
  | Selector((string, _), (Pseudoelement((n, _)), _)) => "Selector(" ++ string ++ "," ++ n ++ ")"
  | Selector((string, _), (Pseudoclass((n, _)), _)) => "Selector(" ++ string ++ "," ++ n ++ ")"
  | Uri(string) => "Uri(" ++ string ++ ")"
  | Operator(string) => "Operator(" ++ string ++ ")"
  | Delim(string) => "Delim(" ++ string ++ ")"
  | Function(name, body) =>
    let body =
      body |> fst |> List.map(render_component_value) |> String.concat(", ");
    "Function(" ++ fst(name) ++ ", " ++ body ++ ")";
  | Hash(string) => "Hash(" ++ string ++ ")"
  | Number(string) => "Number(" ++ string ++ ")"
  | Unicode_range(string) => "Unicode_range(" ++ string ++ ")"
  | Float_dimension((a, b, dimension)) =>
    "Float_dimension("
    ++ a
    ++ ", "
    ++ b
    ++ ", "
    ++ dimension_of_string(dimension)
    ++ ")"
  | Dimension((a, b)) => "Dimension(" ++ a ++ ", " ++ b ++ ")"
  | Variable(variables) =>
    "Variable(" ++ String.concat(", ", variables) ++ ")"
  | _ => assert false
  };
};

let container_lnum = 0;
let pos = Lexing.dummy_pos;
// let css = read_line();
let ast =
  Css_lexer.parse_stylesheet(
    ~container_lnum,
    ~pos,
    "h1 {}",
  );

print_endline(render_stylesheet(ast));
