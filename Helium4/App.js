/* eslint-disable react-native/no-inline-styles */
/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow strict-local
 */

import React, {useEffect, useState} from 'react';
import {
  SafeAreaView,
  ScrollView,
  View,
  Text,
  StatusBar,
  Button,
} from 'react-native';
import Picture from './Picture';
const charts = Array(10).fill(1);

const App = () => {
  const [loaded, setLoaded] = useState(true);
  useEffect(() => {
    StatusBar.setBarStyle('dark-content');
  }, []);
  return (
    <>
      <StatusBar
        barStyle="dark-content"
        backgroundColor="transparent"
        hidden={false}
      />
      <SafeAreaView
        style={{
          flex: 1,
          justifyContent: 'center',
          backgroundColor: 'white',
        }}>
        <View
          style={{
            flex: 1,
            backgroundColor: 'transparent',
          }}>
          {loaded ? (
            <ScrollView style={{flex: 1}}>
              <Text
                style={{
                  fontSize: 40,
                  fontWeight: 'bold',
                  color: '#404040',
                  margin: 16,
                }}>
                Party Mode
              </Text>
              <View
                style={{
                  flexDirection: 'row',
                  flexWrap: 'wrap',
                  justifyContent: 'space-evenly',
                  marginHorizontal: 16,
                }}>
                {charts.map((i, x) => (
                  <Picture key={`${x}`} />
                ))}
              </View>
            </ScrollView>
          ) : null}
          <Button
            style={{marginVertical: 16}}
            title={loaded ? 'Unload me' : 'Load me'}
            onPress={() => setLoaded(!loaded)}
          />
        </View>
      </SafeAreaView>
    </>
  );
};

export default App;
