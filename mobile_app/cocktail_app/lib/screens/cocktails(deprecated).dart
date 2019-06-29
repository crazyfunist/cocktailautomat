import 'package:flutter/material.dart';
import 'cocktailSettings.dart';
import 'cocktail_manager.dart';

class Cocktails extends StatelessWidget {
  final List<String> cocktails;
  Cocktails(this.cocktails);

  @override
  Widget build(BuildContext context) {
    return Column(
      children: cocktails
          .map((element) => Card(
                child: Column(
                  children: <Widget>[
                    Stack(children: <Widget>[
                      Image.asset('assets/indonesia_monkey_wpo.jpg'),
                      Container(
                        height: 250,
                        width: 600,
                        //color: Colors.amber.shade400,
                        child: Visibility(
                          child: Column(
                            children: <Widget>[
                              Text(
                                'HALLO',
                                style: DefaultTextStyle.of(context)
                                    .style
                                    .apply(fontSizeFactor: 1.0),
                              ),
                              Text('BLABLA',
                                  style: DefaultTextStyle.of(context)
                                      .style
                                      .apply(fontSizeFactor: 1.0),
                                  textAlign: TextAlign.center),
                            ],
                          ),
                          visible: true,
                        ),
                      ),
                      FloatingActionButton.extended(
                        icon: Icon(Icons.settings),
                        label: Text(
                          'Setting',
                        ),
                        onPressed: () {
                        },
                      ),
                    ])
                  ],
                ),
              ))
          .toList(),
    );
  }
}
