/* eslint-disable react-native/no-inline-styles */
import React, {useCallback, useRef} from 'react';
import {View} from 'react-native';
import {Canvas} from '@qlik/react-native-helium';
import {createPicasso, Element} from '@qlik/carbon-core';

const {pic} = createPicasso({renderer: 'carbon'});

const settings = {
  scales: {
    x: {data: {field: 'Margin'}},
    y: {data: {field: 'Sales'}},
    col: {
      data: {extract: {field: 'Month'}},
      type: 'color',
    },
  },
  components: [
    {
      // specify how to render the chart
      type: 'axis',
      scale: 'y',
      layout: {
        dock: 'left',
      },
    },
    {
      type: 'axis',
      scale: 'x',
      layout: {
        dock: 'bottom',
      },
    },
    {
      type: 'point',
      data: {
        extract: {
          field: 'Month',
          props: {
            x: {field: 'Margin'},
            y: {field: 'Sales'},
            group: {field: 'Month'},
          },
        },
      },
      settings: {
        x: {scale: 'x'},
        y: {scale: 'y'},
        strokeWidth: 2,
        stroke: '#fff',
        opacity: 0.8,
        fill: {scale: 'col', ref: 'group'},
        size: function () {
          return Math.random() * 2.5;
        },
      },
    },
  ],
};

const Picture = () => {
  const canvas = useRef(undefined);
  const chart = useRef();
  const elementRef = useRef();

  const onCanvas = useCallback(c => {
    canvas.current = c;
    const element = new Element(canvas.current);
    elementRef.current = element;
    chart.current = pic.chart({
      element,
      data: [
        {
          type: 'matrix',
          data: [
            ['Month', 'Sales', 'Margin'],
            ['Jan', 1106, 7],
            ['Feb', 5444, 53],
            ['Mar', 147, 64],
            ['Apr', 7499, 47],
            ['May', 430, 62],
            ['June', 9735, 13],
            ['July', 5832, 13],
            ['Aug', 7435, 15],
            ['Sep', 3467, 35],
            ['Oct', 3554, 78],
            ['Nov', 5633, 23],
            ['Dec', 6354, 63],
          ],
        },
      ],
      settings,
    });
  }, []);

  const onResize = useCallback(() => {
    elementRef.current.clear();
    elementRef.current.resetSize();
    if (chart.current) {
      chart.current.update({element: elementRef.current});
    }
  }, []);

  return (
    <View
      style={{
        height: 300,
        width: '100%',
        maxWidth: 450,
        marginVertical: 8,
        elevation: 8,
        borderColor: '#F0F0F0',
        backgroundColor: 'white',
        borderRadius: 8,
        padding: 4,
        shadowColor: '#000',
        shadowOffset: {
          width: 0,
          height: 2,
        },
        shadowOpacity: 0.25,
        shadowRadius: 3.84,
      }}>
      <Canvas onCanvas={onCanvas} onResized={onResize} />
    </View>
  );
};

export default Picture;
