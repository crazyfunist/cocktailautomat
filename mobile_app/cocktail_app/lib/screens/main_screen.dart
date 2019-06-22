import 'package:flutter/material.dart';
import 'qrScanner.dart';
import 'qr_main_screen.dart';

class MainScreen extends StatefulWidget {
  @override
  _MainScreenState createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> {
  static const double _topSectionTopPadding = 50.0;
  static const double _topSectionBottomPadding = 20.0;

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: _contentWidget(),
      ),
      resizeToAvoidBottomPadding: true,
    );
  }

  Widget _contentWidget() {
    return Container(
      child: Column(
        children: <Widget>[
          Padding(
              padding: const EdgeInsets.only(
              top: _topSectionTopPadding,
              left: 30.0,
              right: 20.0,
              bottom: _topSectionBottomPadding,
            ),
            child: FlatButton(
              child: const Text('scan QR-Code'),
              onPressed: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => QRScanner()),
                );
              },
            ),
          ),
          Padding(
            padding: const EdgeInsets.only(
              top: _topSectionTopPadding,
              left: 30.0,
              right: 20.0,
              bottom: _topSectionBottomPadding,
            ),
            child: FlatButton(
              child: const Text('create QR-Code'),
              onPressed: () {
                Navigator.push(
                  context,
                  MaterialPageRoute(builder: (context) => QRMainScreen()),
                );
              },
            ),
          )
        ],
      ),
    );
  }
}
