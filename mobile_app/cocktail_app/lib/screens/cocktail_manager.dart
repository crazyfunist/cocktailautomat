import 'package:flutter/material.dart';

class CocktailManager extends StatefulWidget {
  final String startingPicture;
  CocktailManager({this.startingPicture});

  @override
  State<StatefulWidget> createState() {
    return _CocktailManagerState();
  }
}

class _CocktailManagerState extends State<CocktailManager> {
  List<Widget> _cocktails = [];
  List<String> cocktails;
  int counter = 0;
  List<bool> vis = [];

  changeState(int count) {}

  getCard(int count) {
    return Card(
      child: Column(
        children: <Widget>[
          Stack(children: <Widget>[
            Image.asset('assets/indonesia_monkey_wpo.jpg'),
            Container(
              height: 250,
              width: 600,
              //color: Colors.amber.shade400,
              child: Visibility(
                  child: Column(children: <Widget>[
                    Row(
                      children: <Widget>[
                        FlatButton(
                          child: Text(
                            '-',
                            style: DefaultTextStyle.of(context)
                                .style
                                .apply(fontSizeFactor: 0.5),
                          ),
                          onPressed: () {
                            print('Minus');
                          },
                        ),
                        Text(
                          'HALLO',
                          style: DefaultTextStyle.of(context)
                              .style
                              .apply(fontSizeFactor: 0.5),
                        ),
                        FlatButton(
                          child: Text(
                            '+',
                            style: DefaultTextStyle.of(context)
                                .style
                                .apply(fontSizeFactor: 0.5),
                          ),
                          onPressed: () {
                            print('Plus');
                          },
                        ),
                      ],
                    ),
                    Row(
                      children: <Widget>[
                        FlatButton(
                          child: Text(
                            '-',
                            style: DefaultTextStyle.of(context)
                                .style
                                .apply(fontSizeFactor: 0.5),
                          ),
                          onPressed: () {
                            print('Minus');
                          },
                        ),
                        Text(
                          'BYE',
                          style: DefaultTextStyle.of(context)
                              .style
                              .apply(fontSizeFactor: 0.5),
                        ),
                        FlatButton(
                          child: Text(
                            '+',
                            style: DefaultTextStyle.of(context)
                                .style
                                .apply(fontSizeFactor: 0.5),
                          ),
                          onPressed: () {
                            print('Plus');
                          },
                        ),
                      ],
                    ),
                  ]),
                  visible: this.vis[count] //false//vis[count],
                  ),
            ),
          ]),
          FloatingActionButton.extended(
            shape: RoundedRectangleBorder(),
            icon: Icon(Icons.settings),
            label: Text(
              'Setting',
            ),
            onPressed: () {
              setState(() {
                vis[count] = !vis[count];
                _cocktails[count] = getCard(count);
                print(vis);
              });
              changeState(count);

              //_cocktails.add('Pic' + counter.toString());
            },
          ),
        ],
      ),
    );
  }

  addCard(int count) {
    vis.add(true);
    _cocktails.add(getCard(count));
  }

  getCock() {
    return _cocktails;
  }

  @override
  Widget build(BuildContext context) {
    //print(_cocktails);
    //vis.add(true);
    return ListView(
      children: <Widget>[
        Container(
          margin: EdgeInsets.all(10.0),
          child: RaisedButton(
            color: Theme.of(context).primaryColor,
            onPressed: () {
              setState(() {
                //vis[0] = !vis[0];
                addCard(counter);
                counter++;
                //_cocktails.add('Pic' + counter.toString());
              });
            },
            child: Text('Add Picture'),
          ),
        ), //Visibility(child: Text('Hello'), visible: vis[0],),
        Column(children: getCock()) //_cocktails)
      ],
    );
  }
}
