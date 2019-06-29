import 'package:flutter/material.dart';

class CocktailSettings extends StatefulWidget {
  final String startingPicture;
  CocktailSettings({this.startingPicture});

  @override
  State<StatefulWidget> createState() {
    return _CocktailSettings();
  }
}

class _CocktailSettings extends State<CocktailSettings> {
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        child: Column(
          children: <Widget>[
            Padding(
                padding: const EdgeInsets.only(
                  top: 50.0,
                  left: 30.0,
                  right: 20.0,
                  bottom: 50.0,
                ),
                child: Text('Hello'))
          ],
        ),
      ),
    );
  }
}
